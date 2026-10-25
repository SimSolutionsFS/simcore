#include "simcore/win.h"

#include <stdlib.h>
#include <XPLMDisplay.h>

// "Dummy" callbacks. Adapted from https://developer.x-plane.com/code-sample/hello-world-sdk-3/
static void base_draw(XPLMWindowID in_window_id, void *in_refcon) {}

static int base_click(XPLMWindowID in_window_id, int x, int y, int is_down, void *in_refcon) {
	return 0;
}

static XPLMCursorStatus base_cursor(XPLMWindowID in_window_id, int x, int y, void *in_refcon) {
	return xplm_CursorDefault;
}

static int base_wheel(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void *in_refcon) {
	return 0;
}

static void base_key(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void *in_refcon,
					 int losing_focus) {}

// Real functions
void sc_win_new(sc_win_t *in_win, int width, int height) {
	in_win->width = width;
	in_win->height = height;
	in_win->buffer = malloc(width * height * 4 * sizeof(unsigned char));

	in_win->draw_func = base_draw;
	in_win->click_func = base_click;
	in_win->wheel_func = base_wheel;
	in_win->key_func = base_key;
	in_win->cursor_func = base_cursor;
}

void sc_win_set_cb(sc_win_t *in_win, const XPLMDrawWindow_f draw_cb, const XPLMHandleMouseClick_f click_cb,
				   const XPLMHandleCursor_f cursor_cb, const XPLMHandleMouseWheel_f wheel_cb,
				   const XPLMHandleKey_f key_cb) {
	// just a little bit of repetition nothing to see here
	if (draw_cb != NULL) {
		in_win->draw_func = draw_cb;
	}
	else {
		in_win->draw_func = base_draw;
	}

	if (click_cb != NULL) {
		in_win->click_func = click_cb;
	}
	else {
		in_win->click_func = base_click;
	}

	if (cursor_cb != NULL) {
		in_win->cursor_func = cursor_cb;
	}
	else {
		in_win->cursor_func = base_cursor;
	}

	if (wheel_cb != NULL) {
		in_win->wheel_func = wheel_cb;
	}
	else {
		in_win->wheel_func = base_wheel;
	}

	if (key_cb != NULL) {
		in_win->key_func = key_cb;
	}
	else {
		in_win->key_func = base_key;
	}
}

void sc_win_create(sc_win_t *in_win, const char *title) {
	sc_win_create2(in_win, title, 50, 150);
}

void sc_win_create2(sc_win_t *in_win, const char *title, const int win_left, const int win_down) {
	in_win->params.structSize = sizeof(in_win->params);
	in_win->params.visible = 1;
	in_win->params.drawWindowFunc = in_win->draw_func;
	in_win->params.handleMouseClickFunc = in_win->click_func;
	in_win->params.handleRightClickFunc = in_win->click_func;
	in_win->params.handleMouseWheelFunc = in_win->wheel_func;
	in_win->params.handleKeyFunc = in_win->key_func;
	in_win->params.handleCursorFunc = in_win->cursor_func;
	in_win->params.refcon = NULL;
	in_win->params.layer = xplm_WindowLayerFloatingWindows;
	in_win->params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

	int left, bottom, right, top;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	in_win->params.left = left + win_left;
	in_win->params.bottom = bottom + win_down;
	in_win->params.right = in_win->params.left + in_win->width - 20;
	in_win->params.top = in_win->params.bottom + in_win->height - 20;
	in_win->params.contentType = xplm_WindowContentTypePanelGraphics;

	in_win->id = XPLMCreateWindowEx(&in_win->params);

	XPLMSetWindowPositioningMode(in_win->id, xplm_WindowPositionFree, -1);
	XPLMSetWindowTitle(in_win->id, title);
}

void sc_win_set_size(const sc_win_t *in_win, const int min_w, const int min_h, const int max_w, const int max_h) {
	XPLMSetWindowResizingLimits(in_win->id, min_w - 20, min_h - 20, max_w - 20, max_h - 20);
}
