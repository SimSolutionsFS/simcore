#include "simcore/persist.h"

#include <errno.h>
#include <XPLMGraphics.h>
#include <XPLMPlanes.h>
#include <XPLMProcessing.h>
#include <XPLMUtilities.h>
#include <acfutils/cmd.h>
#include <acfutils/conf.h>
#include <acfutils/dr.h>
#include <acfutils/geom.h>
#include <acfutils/perf.h>
#include <xpdraw/fonts.h>
#include <xpdraw/tools.h>
#include <xpdraw/windows.h>
#include <xpdraw/xpdraw.h>

#ifdef WIN32
#include <io.h>
#endif

#define UPDATE_RATE_SEC 30
#define PERSIST_DIST_LIM 5.0f
double dist_to_save = -1;

#ifdef WIN32
#define ACCESS(x, y) _access(x, y)
#define MKDIR(x) mkdir(x)
#else
#define ACCESS(x, y) access(x, y)
#define MKDIR(x) mkdir(x, 0777)
#endif

// Flight loop stuff, important for stuff to run well
XPLMFlightLoopID persist_fl;
char *persist_fp;
conf_t *persist_conf;
dr_t tmp_dr;
int tmp_val_i;
float tmp_val_f;

// Config settings & basic state stuff
int load_save = 0;
int finish_load = 0;

// Datarefs for the current state of the aircraft.
dr_t dr_acf_lat;
dr_t dr_acf_lon;
dr_t dr_acf_hdg;
dr_t dr_acf_spd;
dr_t dr_acf_alt;
dr_t dr_eng_running;
dr_t dr_eng_thro;

// ...and the values for those datarefs
double acf_lat;
double acf_lon;
float acf_true_hdg;
float acf_spd_mps;
float acf_alt_ft;
int eng_running;
float eng_thro;

/*
 * Window stuff if we have to prompt.
 * Unused if no prompt necessary.
 */
#define WIN_H 250
#define WIN_W 600

#define WIN_CLR_H (xpd_color_t){ 0.35, 0.35, 0.35, 1 }
#define WIN_CLR_BUT_PRI (xpd_color_t){ 0.23, 0.49, 0.65, 1 }
#define WIN_CLR_BUT_SEC WIN_CLR_H

xpd_win_t pmpt_win;

xpd_font_face_t win_header_font;
xpd_font_face_t win_text_font;

char win_vrb_text[512];

// Flight loop that constantly loads the state of the aircraft (every UPDATE_RATE_SEC) and updates its status in the file.
float persist_fl_ref(float elapsedMe, float elapsedSim, int counter, void *refcon) {
	UNUSED(elapsedMe);
	UNUSED(elapsedSim);
	UNUSED(counter);
	UNUSED(refcon);

	for (int i = 0; i < sw_get_array_size(); i++) {
		sw_t tmp = sw_get_idx(i);

		if (!tmp.spring) {
			ASSERT(tmp.ref != NULL);
			conf_set_i(persist_conf, tmp.ref, tmp.state);
		}
	}

	conf_set_f(persist_conf, "location/lat", dr_getf(&dr_acf_lat));
	conf_set_f(persist_conf, "location/lon", dr_getf(&dr_acf_lon));
	conf_set_f(persist_conf, "location/hdg", dr_getf(&dr_acf_hdg));
	conf_set_f(persist_conf, "location/tas", dr_getf(&dr_acf_spd));
	conf_set_f(persist_conf, "location/alt", dr_getf(&dr_acf_alt));
	conf_set_i(persist_conf, "engine/running", dr_getf(&dr_eng_running));
	conf_set_f(persist_conf, "engine/throttle", dr_getf(&dr_eng_thro));

	conf_write_file(persist_conf, persist_fp);
	return UPDATE_RATE_SEC;
}

// Loads the saved data into the sim.
void persist_load_save(void) {
	logMsg("Restoring state...");
	for (int i = 0; i < sw_get_array_size(); i++) {
		sw_t tmp = sw_get_idx(i);

		if (!tmp.spring) {
			ASSERT(tmp.ref != NULL);
			conf_get_i(persist_conf, tmp.ref, &tmp.state);
		}
	}

	XPLMPlaceUserAtLocation(
		acf_lat,
		acf_lon,
		acf_alt_ft,
		acf_true_hdg,
		acf_spd_mps
	);

	if (eng_running == 1) {
		XPLMCommandOnce(cmd_find("sim/operation/auto_start"));
	}

	dr_setf(&dr_eng_thro, eng_thro);

	float tmp_pk_brake;
	conf_get_f(persist_conf, "sim/cockpit2/controls/wheel_brake_ratio", &tmp_pk_brake);
	if (tmp_pk_brake > 0.5) {
		XPLMCommandOnce(cmd_find("sim/flight_controls/brakes_toggle_max"));
	}
}

// Start the flight loop that constantly updates the current position
void persist_fl_start(void) {
	// Create a flight loop to constantly refresh stuff
	XPLMCreateFlightLoop_t persistenceFLParams = {
		sizeof(XPLMCreateFlightLoop_t),
		xplm_FlightLoop_Phase_AfterFlightModel,
		persist_fl_ref,
		NULL
	};

	persist_fl = XPLMCreateFlightLoop(&persistenceFLParams);
	XPLMScheduleFlightLoop(persist_fl, UPDATE_RATE_SEC, 1);
}

int kill_win = 0;
XPLMFlightLoopID kill_fl;

void pmpt_win_render(XPLMWindowID in_window_id, void *inRefcon) {
	UNUSED(inRefcon);

	XPLMBindTexture2d(pmpt_win.texNum, 0);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIN_H, WIN_W, GL_RGBA, GL_UNSIGNED_BYTE, pmpt_win.buffer);
	XPLMSetGraphicsState(0, 0, 0, 0, 1, 1, 0);
	int windowLeft, windowBottom;
	XPLMGetWindowGeometry(in_window_id, &windowLeft, NULL, NULL, &windowBottom);

	xpd_set_anchor(windowLeft - 10, windowBottom - 10);

	xpd_draw_rect(0, 0, 600, 250, XPD_COLOR_BLACK);
	xpd_draw_rect(0, 200, 600, 50, WIN_CLR_H);
	xpd_text_draw(&win_header_font, "Restore your last state?", 10, 214, XPD_ALIGN_L, XPD_COLOR_WHITE);

	sprintf(win_vrb_text, "You are %.2f nm from your last session.", dist_to_save);
	xpd_text_draw(&win_text_font, win_vrb_text, 15, 165, XPD_ALIGN_L, XPD_COLOR_WHITE);
	xpd_text_draw(&win_text_font, "Would you like to restore your position and", 15, 115, XPD_ALIGN_L, XPD_COLOR_WHITE);
	xpd_text_draw(&win_text_font, "general state?", 15, 85, XPD_ALIGN_L, XPD_COLOR_WHITE);

	xpd_draw_rect(20, 20, 130, 35, WIN_CLR_BUT_PRI);
	xpd_text_draw(&win_text_font, "Yes", 85, 28, XPD_ALIGN_C, XPD_COLOR_WHITE);

	xpd_draw_rect(170, 20, 130, 35, WIN_CLR_BUT_SEC);
	xpd_text_draw(&win_text_font, "No", 235, 28, XPD_ALIGN_C, XPD_COLOR_WHITE);
}

int pmpt_win_cb(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void *inRefcon) {
	UNUSED(inMouse);
	UNUSED(inRefcon);

	int windowLeft, windowBottom;
	XPLMGetWindowGeometry(inWindowID, &windowLeft, NULL, NULL, &windowBottom);

	x -= windowLeft;
	y -= windowBottom;

	if (20 < y && y < 55) {
		if (20 < x && x < 150) {
			// Load the previous save and start the system.
			persist_load_save();
			persist_fl_start();
			kill_win = 1;
		}
		else if (170 < x && x < 300) {
			// Don't load the previous save, but still start the system.
			persist_fl_start();
			kill_win = 1;
		}
	}

	return 1;
}

/*
 * What you are about to read might be the dumbest part of this code base written since I graduated highschool.
 *
 * The following code works around a CTD when calling XPLMDestroyWindow() from a click callback.
 *
 * That is the only reason this flight loop exists.
 */
float the_kill_fl_ref(float elapsedMe, float elapsedSim, int counter, void *refcon) {
	UNUSED(elapsedMe);
	UNUSED(elapsedSim);
	UNUSED(counter);
	UNUSED(refcon);

	if (kill_win == 1) {
		XPLMDestroyWindow(pmpt_win.windowID);
		XPLMDestroyFlightLoop(kill_fl);
		kill_win = 0;
	}

	return -1;
}

void pmpt_win_create(void) {
	// schedule the KILL flight loop.
	XPLMCreateFlightLoop_t persistenceFLParams = {
		sizeof(XPLMCreateFlightLoop_t),
		xplm_FlightLoop_Phase_AfterFlightModel,
		the_kill_fl_ref,
		NULL
	};

	kill_fl = XPLMCreateFlightLoop(&persistenceFLParams);
	XPLMScheduleFlightLoop(kill_fl, -1, 1);

	// Create window object
	xpd_win_new(&pmpt_win, WIN_W, WIN_H);

	// Load callbacks
	xpd_win_set_draw_cb(&pmpt_win, pmpt_win_render);
	xpd_win_set_click_cb(&pmpt_win, pmpt_win_cb);

	// Display window
	xpd_win_create(&pmpt_win, "Restore Save?");
	xpd_win_resize_lims(&pmpt_win, WIN_W, WIN_H, WIN_W, WIN_H);
}

void persist_init(const char *in_folder_pth) {
	// Start finding path to save
	persist_fp = malloc(sizeof(char) * 512);

	XPLMGetPrefsPath(persist_fp);
	persist_fp[strlen(persist_fp) - 28] = '\0';
	persist_fp = xpd_tools_constr(persist_fp, in_folder_pth);

	// Load fonts
	char *win_header_font_pth = xpd_tools_constr(xpd_tools_xp_fp(), "/Resources/fonts/Roboto-Bold.ttf");
	char *win_text_font_path = xpd_tools_constr(xpd_tools_xp_fp(), "/Resources/fonts/Roboto-Regular.ttf");
	xpd_font_load(&win_header_font, win_header_font_pth, 26);
	xpd_font_load(&win_text_font, win_text_font_path, 18);
	free(win_header_font_pth);
	free(win_text_font_path);
}

// Runs when we should start the persistance system
// We might want to defer this until the dialog box is closed (if it opened)
void persist_begin(void) {
	if (load_save) {
		conf_get_d(persist_conf, "location/lat", &acf_lat);
		conf_get_d(persist_conf, "location/lon", &acf_lon);
		conf_get_f(persist_conf, "location/hdg", &acf_true_hdg);
		conf_get_f(persist_conf, "location/tas", &acf_spd_mps);
		conf_get_f(persist_conf, "location/alt", &acf_alt_ft);
		conf_get_i(persist_conf, "engine/running", &eng_running);
		conf_get_f(persist_conf, "engine/throttle", &eng_thro);

		dist_to_save = MET2NM(
			gc_distance(GEO_POS2(acf_lat, acf_lon), GEO_POS2(dr_getf(&dr_acf_lat), dr_getf(&dr_acf_lon))));
		logMsg("We are %fnm from the last save! Currently at %f, %f", dist_to_save, dr_getf(&dr_acf_lat),
			   dr_getf(&dr_acf_lon));

		if (dist_to_save <= PERSIST_DIST_LIM) {
			persist_load_save();
		}
		else {
			pmpt_win_create();
			return;
		}
	}

	persist_fl_start();
}

// Runs when the aircraft first starts.
void persist_on_load(void) {
	// Check to see if the DA40 folder exists, if not create it.
	if (ACCESS(persist_fp, R_OK) != 0) {
		logMsg("Save directory does not exist. Creating!");
		const int check = MKDIR(persist_fp);
		const int errsv = errno;
		if (check != 0) {
			logMsg("Attempted to create directory with error code %i. The attempted path was %s.", errsv, persist_fp);
			assert(check == 0);
		}
	}

	// Calculate correct name for per-livery save
	dr_t dr_liv_pth;
	dr_find(&dr_liv_pth, "sim/aircraft/view/acf_livery_path");

	int len = dr_gets(&dr_liv_pth, NULL, 0);
	char *liv_pth;
	if (len == 0) {
		liv_pth = malloc(sizeof(char) * 8);
		strcpy(liv_pth, "default");
	}
	else {
		liv_pth = malloc(len);
		dr_gets(&dr_liv_pth, liv_pth, len);
		liv_pth = XPLMExtractFileAndPath(liv_pth);
	}

	// Finalize the path to save file
	sprintf(persist_fp, "%s%s.cfg", persist_fp, liv_pth);
	//free(liv_pth); DON'T UNCOMMENT THIS OR ELSE WINDOWS WILL CRASH. THIS REMAINS HERE HAS A REMINDER TO THE FUTURE TRAVELERS OF THIS CODE.

	// Do rest of loading
	logMsg("Persistence system enabled.");

	// Attempt to fetch a save, and create a new one if it does not exist
	if (ACCESS(persist_fp, F_OK) == 0) {
		load_save = 1;
		logMsg("Save data found!");
		persist_conf = conf_read_file(persist_fp, NULL);
	}
	else {
		load_save = 0;
		logMsg("Save data not found!");
		persist_conf = conf_create_empty();
	}

	// Load some state datarefs
	dr_find(&dr_acf_lat, "sim/flightmodel/position/latitude");
	dr_find(&dr_acf_lon, "sim/flightmodel/position/longitude");
	dr_find(&dr_acf_hdg, "sim/flightmodel/position/true_psi");
	dr_find(&dr_acf_spd, "sim/flightmodel/position/true_airspeed");
	dr_find(&dr_acf_alt, "sim/flightmodel/position/elevation");
	dr_find(&dr_eng_running, "sim/flightmodel/engine/ENGN_running");
	dr_find(&dr_eng_thro, "sim/flightmodel/engine/ENGN_thro");

	persist_begin();
}

// Kill everything we created when the sim shuts down.
void persist_stop(void) {
	XPLMDestroyFlightLoop(persist_fl);

	free(persist_fp);
	conf_free(persist_conf);
}
