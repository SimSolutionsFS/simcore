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

	sw_t *i = inRefcon;

	if (inPhase == xplm_CommandBegin) {
		if (i->state == 0) {
			i->act_gain = SWITCH_GAIN;
		}
		else {
			i->act_gain = -SWITCH_GAIN;
		}

		i->state = !i->state;
	}
	else if ((i->spring) && (inPhase == xplm_CommandEnd)) {
		i->act_gain = -SWITCH_GAIN;
		i->state = 0;
	}

	if (i->dr_state_exists) {
		dr_seti(&i->dr_state, i->state);
	}

	return xplm_CommandContinue;
}

int sw_cb_l(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon) {
	UNUSED(inCommand);

	sw_t *i = inRefcon;

	if ((inPhase == xplm_CommandBegin) && (i->state > i->min)) {
		i->act_gain = -SWITCH_GAIN;
		i->state -= 1;
	}
	else if ((inPhase == xplm_CommandEnd) && (i->spring)) {
		i->act_gain = SWITCH_GAIN;
		i->state = 0;
	}

	if (i->dr_state_exists) {
		dr_seti(&i->dr_state, i->state);
	}

	return xplm_CommandContinue;
}

int sw_cb_r(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void *inRefcon) {
	UNUSED(inCommand);

	sw_t *i = inRefcon;

	if ((inPhase == xplm_CommandBegin) && (i->state < i->max)) {
		i->act_gain = SWITCH_GAIN;
		i->state += 1;
	}
	else if ((inPhase == xplm_CommandEnd) && ((i->starter) || (i->spring)) && (i->state == i->max)) {
		i->act_gain = -SWITCH_GAIN;
		i->state -= 1;
	}

	if (i->dr_state_exists) {
		dr_seti(&i->dr_state, i->state);
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

// Callbacks to update the state of the switch. Used by X-Plane and other stakeholders
int sw_get_state(switch_t inRefcon) {
	return inRefcon->state;
}

void sw_write_state(switch_t inRefcon, int inValue) {
	inRefcon->state = inValue;
}

float sw_get_anim(switch_t inRefcon) {
	return inRefcon->anim_pos;
}

switch_t sw_append(void) {
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
		all_sw = realloc(all_sw, sizeof(sw_t) * all_sw_size);
		assert(all_sw);
		idx = all_sw_size - 1;
	}

	all_sw[idx].state = 0;
	all_sw[idx].anim_pos = 0;
	all_sw[idx].act_gain = 0;
	all_sw[idx].dr_state_exists = 0;

	return &all_sw[idx];
}

switch_t sw_init(const char *dr_name, const char *dr_anim_name, const char *cmd_name, const char *cmd_desc, const int spring) {
	// Initialize the switch
	sw_t *idx = sw_append();
	idx->type = SW_BASIC;
	idx->spring = spring;

	// Register commands
	if (cmd_desc == NULL) {
		cmd_desc = "";
	}

	idx->cmd_toggle = XPLMFindCommand(cmd_name);
	if (idx->cmd_toggle == NULL) {
		idx->cmd_toggle = XPLMCreateCommand(cmd_name, cmd_desc);
	}
	XPLMRegisterCommandHandler(idx->cmd_toggle, sw_basic_cb, 0, (void *)idx);

	// Register datarefs
	if (dr_name != NULL) {
		unsigned int result = dr_find(&idx->dr_state, "%s", dr_name);
		if (result == 0) {
			dr_create_i(&idx->dr_state, &idx->state, 1, "%s", dr_name);
		}
		else {
			idx->dr_state_exists = 1;
		}
	}

	if (dr_anim_name != NULL) {
		dr_create_f(&idx->dr_anim, &idx->anim_pos, 0, "%s", dr_anim_name);
	}

	return idx;
}

switch_t sw_init2(const char *dr_name, const char *dr_anim_name, const char *cmd_name_l, const char *cmd_desc_l,
					   const char *cmd_name_r, const char *cmd_desc_r, const int min_range, const int max_range,
					   const int default_value,
					   const int starter, const int spring) {
	// Initialize the switch
	sw_t *idx = sw_append();
	idx->type = SW_MULTI;
	idx->spring = spring;

	assert(!(spring && starter));

	idx->state = default_value;
	idx->min = min_range;
	idx->max = max_range;
	idx->starter = starter;

	if (cmd_desc_l == NULL) {
		cmd_desc_l = "";
	}

	if (cmd_desc_r == NULL) {
		cmd_desc_r = "";
	}

	// Register commands
	idx->cmd_toggle_l = XPLMFindCommand(cmd_name_l);
	if (idx->cmd_toggle_l == NULL) {
		idx->cmd_toggle_l = XPLMCreateCommand(cmd_name_l, cmd_desc_l);
	}
	XPLMRegisterCommandHandler(idx->cmd_toggle_l, sw_cb_l, 0, (void *)idx);

	idx->cmd_toggle_r = XPLMFindCommand(cmd_name_r);
	if (idx->cmd_toggle_r == NULL) {
		idx->cmd_toggle_r = XPLMCreateCommand(cmd_name_r, cmd_desc_r);
	}
	XPLMRegisterCommandHandler(idx->cmd_toggle_r, sw_cb_r, 0, (void *)idx);

	// Register datarefs
	if (dr_name != NULL) {
		unsigned int result = dr_find(&idx->dr_state, "%s", dr_name);
		if (result == 0) {
			dr_create_i(&idx->dr_state, &idx->state, 1, "%s", dr_name);
		}
		else {
			idx->dr_state_exists = 1;
		}
	}

	if (dr_anim_name != NULL) {
		dr_create_f(&idx->dr_anim, &idx->anim_pos, 0, "%s", dr_anim_name);
	}

	return idx;
}

void sw_destroy(void) {
	// Unregister all commands
	for (int i = 0; i < all_sw_size; i++) {
		switch (all_sw[i].type) {
			case SW_MULTI:
				XPLMUnregisterCommandHandler(all_sw[i].cmd_toggle_l, sw_cb_l, 0, &all_sw[i]);
				XPLMUnregisterCommandHandler(all_sw[i].cmd_toggle_r, sw_cb_r, 0, &all_sw[i]);
				break;
			default:
				XPLMUnregisterCommandHandler(all_sw[i].cmd_toggle, sw_basic_cb, 0, &all_sw[i]);
		}
	}

	// Free all memory
	free(all_sw);
}
