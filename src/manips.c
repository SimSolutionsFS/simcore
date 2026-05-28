#include "simcore/manips.h"

#include <assert.h>
#include <stdlib.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <acfutils/cmd.h>
#include <acfutils/dr.h>

#define SWITCH_GAIN 0.2f

sw_t *all_sw;
int all_sw_size = 0;

// Switch Callbacks
int sw_basic_cb(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon) {
	UNUSED(inCommand);

	int i = (int)inRefcon;

	if (inPhase == xplm_CommandBegin) {
		if (all_sw[i].state == 0) {
			all_sw[i].act_gain = SWITCH_GAIN;
		}
		else {
			all_sw[i].act_gain = -SWITCH_GAIN;
		}

		all_sw[i].state = !all_sw[i].state;
	}
	else if ((all_sw[i].spring) && (inPhase == xplm_CommandEnd)) {
		all_sw[i].act_gain = -SWITCH_GAIN;
		all_sw[i].state = 0;
	}

	if (all_sw[i].dr_state_exists) {
		dr_seti(&all_sw[i].dr_state, all_sw[i].state);
	}

	return xplm_CommandContinue;
}

int sw_cb_l(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon) {
	UNUSED(inCommand);

	int i = (int)inRefcon;

	if ((inPhase == xplm_CommandBegin) && (all_sw[i].state > all_sw[i].min)) {
		all_sw[i].act_gain = -SWITCH_GAIN;
		all_sw[i].state -= 1;
	}
	else if ((inPhase == xplm_CommandEnd) && (all_sw[i].spring)) {
		all_sw[i].act_gain = SWITCH_GAIN;
		all_sw[i].state = 0;
	}

	if (all_sw[i].dr_state_exists) {
		dr_seti(&all_sw[i].dr_state, all_sw[i].state);
	}

	return xplm_CommandContinue;
}

int sw_cb_r(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon) {
	UNUSED(inCommand);

	int i = (int)inRefcon;

	if ((inPhase == xplm_CommandBegin) && (all_sw[i].state < all_sw[i].max)) {
		all_sw[i].act_gain = SWITCH_GAIN;
		all_sw[i].state += 1;
	}
	else if ((inPhase == xplm_CommandEnd) && ((all_sw[i].starter) || (all_sw[i].spring)) && (
				 all_sw[i].state == all_sw[i].max)) {
		all_sw[i].act_gain = -SWITCH_GAIN;
		all_sw[i].state -= 1;
	}

	if (all_sw[i].dr_state_exists) {
		dr_seti(&all_sw[i].dr_state, all_sw[i].state);
	}

	return xplm_CommandContinue;
}

// Refresh all animation datarefs
void sw_ref(void) {
	for (int i = 0; i < all_sw_size; i++) {
		if (all_sw[i].act_gain != 0) {
			all_sw[i].anim_pos += all_sw[i].act_gain;

			// We don't want to overshoot the actual current state of the switch
			if (((all_sw[i].act_gain < 0) && (all_sw[i].anim_pos < (float)all_sw[i].state)) || (
					(all_sw[i].act_gain > 0) && (all_sw[i].anim_pos > (float)all_sw[i].state))) {
				all_sw[i].anim_pos = (float)all_sw[i].state;
				all_sw[i].act_gain = 0;
			}
		}
		else if (all_sw[i].anim_pos != (float)all_sw[i].state) {
			all_sw[i].anim_pos = (float)all_sw[i].state;
		}
	}
}

// Callbacks to update the state of the switch. Used by X-Plane
int sw_get_state_int(void *inRefcon) {
	return all_sw[(int)inRefcon].state;
}

void sw_set_state_int(void *inRefcon, int in_num) {
	all_sw[(int)inRefcon].state = in_num;
}

float sw_get_anim_int(void *inRefcon) {
	return all_sw[(int)inRefcon].anim_pos;
}

// Callbacks to update & get the state of the switch. Used instead of pointers to prevent issues
int sw_get_state(switch_t inRefcon) {
	return all_sw[inRefcon].state;
}

float sw_get_anim(switch_t inRefcon) {
	return all_sw[inRefcon].anim_pos;
}

void sw_set_state(switch_t inRefcon, int in_num) {
	all_sw[inRefcon].state = in_num;
}

sw_t *sw_create_blank(void) {
	int idx;
	if (all_sw == NULL) {
		// Allocate initial memory if the array doesn't already exist
		all_sw = malloc(sizeof(sw_t));
		all_sw_size = 1;
		idx = 0;
	}
	else {
		// Expand the array to support our new switch
		all_sw_size++;
		sw_t *all_sw_tmp = realloc(all_sw, sizeof(sw_t) * all_sw_size);
		assert(all_sw_tmp != NULL);
		all_sw = all_sw_tmp;
		idx = all_sw_size - 1;
	}

	all_sw[idx].state = 0;
	all_sw[idx].anim_pos = 0;
	all_sw[idx].act_gain = 0;
	all_sw[idx].dr_state_exists = 0;
	all_sw[idx].index = idx;

	return &all_sw[idx];
}

sw_t **sw_init() {
	return &all_sw;
}

switch_t sw_new(char *dr_name, const char *dr_anim_name, const char *cmd_name, const char *cmd_desc, const int spring) {
	// Initialize the switch
	sw_t *sw = sw_create_blank();
	sw->type = SW_BASIC;
	sw->spring = spring;
	sw->ref = dr_name;

	if (spring == 0) {
		ASSERT(dr_name != NULL);
	}

	// Register commands
	if (cmd_desc == NULL) {
		cmd_desc = cmd_name;
	}

	sw->cmd_toggle = XPLMFindCommand(cmd_name);
	if (sw->cmd_toggle == NULL) {
		sw->cmd_toggle = XPLMCreateCommand(cmd_name, cmd_desc);
	}
	XPLMRegisterCommandHandler(sw->cmd_toggle, sw_basic_cb, 0, (void *)sw->index);

	// Register datarefs
	if (dr_name != NULL) {
		unsigned int result = dr_find(&sw->dr_state, "%s", dr_name);
		if (result == 0) {
			XPLMRegisterDataAccessor(
				dr_name,
				xplmType_Int,
				0,
				sw_get_state_int,
				sw_set_state_int,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				(void *)sw->index,
				NULL
			);
		}
		else {
			sw->dr_state_exists = 1;
			sw->state = dr_geti(&sw->dr_state);
			sw->anim_pos = (float)sw->state;
		}
	}

	if (dr_anim_name != NULL) {
		XPLMRegisterDataAccessor(
			dr_anim_name,
			xplmType_Float,
			0,
			NULL,
			NULL,
			sw_get_anim_int,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(void *)sw->index,
			NULL
		);
	}

	return sw->index;
}

switch_t sw_new2(char *dr_name, const char *dr_anim_name, const char *cmd_name_l, const char *cmd_desc_l,
				 const char *cmd_name_r, const char *cmd_desc_r, const int min_range, const int max_range,
				 const int default_value, const int starter) {
	// Initialize the switch
	sw_t *sw = sw_create_blank();
	sw->type = SW_MULTI;
	sw->spring = 0;
	sw->ref = dr_name;

	ASSERT(dr_name != NULL);

	sw->state = default_value;
	sw->min = min_range;
	sw->max = max_range;
	sw->starter = starter;

	if (cmd_desc_l == NULL) {
		cmd_desc_l = cmd_name_l;
	}

	if (cmd_desc_r == NULL) {
		cmd_desc_r = cmd_name_r;
	}

	// Register commands
	sw->cmd_toggle_l = XPLMFindCommand(cmd_name_l);
	if (sw->cmd_toggle_l == NULL) {
		sw->cmd_toggle_l = XPLMCreateCommand(cmd_name_l, cmd_desc_l);
	}
	XPLMRegisterCommandHandler(sw->cmd_toggle_l, sw_cb_l, 0, (void *)sw->index);

	sw->cmd_toggle_r = XPLMFindCommand(cmd_name_r);
	if (sw->cmd_toggle_r == NULL) {
		sw->cmd_toggle_r = XPLMCreateCommand(cmd_name_r, cmd_desc_r);
	}
	XPLMRegisterCommandHandler(sw->cmd_toggle_r, sw_cb_r, 0, (void *)sw->index);

	// Register datarefs
	if (dr_name != NULL) {
		unsigned int result = dr_find(&sw->dr_state, "%s", dr_name);
		if (result == 0) {
			XPLMRegisterDataAccessor(
				dr_name,
				xplmType_Int,
				0,
				sw_get_state_int,
				sw_set_state_int,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				(void *)sw->index,
				NULL
			);
		}
		else {
			sw->dr_state_exists = 1;
			sw->state = dr_geti(&sw->dr_state);
			sw->anim_pos = (float)sw->state;
		}
	}

	if (dr_anim_name != NULL) {
		XPLMRegisterDataAccessor(
			dr_anim_name,
			xplmType_Float,
			0,
			NULL,
			NULL,
			sw_get_anim_int,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(void *)sw->index,
			NULL
		);
	}

	return sw->index;
}

void sw_destroy(void) {
	// Unregister all commands
	for (int i = 0; i < all_sw_size; i++) {
		switch (all_sw[i].type) {
			case SW_MULTI:
				XPLMUnregisterCommandHandler(all_sw[i].cmd_toggle_l, sw_cb_l, 0, (void *)i);
				XPLMUnregisterCommandHandler(all_sw[i].cmd_toggle_r, sw_cb_r, 0, (void *)i);
				break;
			default:
				XPLMUnregisterCommandHandler(all_sw[i].cmd_toggle, sw_basic_cb, 0, (void *)i);
		}
	}

	// Free all memory
	free(all_sw);
}


int sw_get_array_size(void) {
	return all_sw_size;
}
