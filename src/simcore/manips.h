#ifndef LIBSWITCH_H
#define LIBSWITCH_H

#include <XPLMUtilities.h>
#include <acfutils/dr.h>

typedef enum { SW_BASIC, SW_MULTI } sw_type_t;

struct {
	// Internal
	sw_type_t type;
	int index;

	// Data for basic switches
	int state;
	int spring;
	float act_gain;
	float anim_pos;
	dr_t dr_state;
	int dr_state_exists;
	dr_t dr_anim;
	XPLMCommandRef cmd_toggle;

	// Data for multi-position switches
	int min;
	int max;
	int starter;
	XPLMCommandRef cmd_toggle_l;
	XPLMCommandRef cmd_toggle_r;
} typedef sw_t;

/**
 * @brief Exported switch type.
 *
 * Previously, this was an int that represented an index in an array.
 * Currently, this represents a pointer to that array entry.
 */
typedef sw_t* switch_t;

/**
 * @brief Returns the necessary pointer for the updated persistence system. Yay!
 *
 * @return Pointer to the array that contains all switches.
 */
sw_t **sw_init(void);

/**
 * @brief Initialize a basic, 2 position (on/off) switch
 *
 * @param dr_name Name of the switches state dataref. This is for systems, and should not be used for animations.
 * @param dr_anim_name Name of the switch's animation dataref. This is for animations only, and should not be used for animations.
 * @param cmd_name Name of the switch's toggle command. This should be used in clickspots.
 * @param cmd_desc Description to show up in the X-Plane UI.
 * @param spring Does the switch return to 0 once released?
 *
 * @return An ID for the switch.
 */
switch_t sw_new(const char *dr_name, const char *dr_anim_name, const char *cmd_name, const char *cmd_desc, int spring);

/**
 * @brief Initialize a switch with multiple positions.
 *
 * @param dr_name Name of the switches state dataref. This is for systems, and should not be used for animations.
 * @param dr_anim_name Name of the switch's animation dataref. This is for animations only, and should not be used for animations.
 * @param cmd_name_l Name of the switch's toggle command. This should be used in clickspots.
 * @param cmd_desc_l Description to show up in the X-Plane UI.
 * @param cmd_name_r Name of the switch's toggle command. This should be used in clickspots.
 * @param cmd_desc_r Description to show up in the X-Plane UI.
 * @param min_range Minimum value the switch can be.
 * @param max_range Maximum value the switch can be.
 * @param default_value Default value of the switch.
 * @param starter If true, the last position will be spring-loaded.
 * @param spring Does the switch return to 0 once released? Cannot be used with `starter`.
 *
 * @return An ID for the switch.
 */
switch_t sw_new2(const char *dr_name, const char *dr_anim_name, const char *cmd_name_l, const char *cmd_desc_l,
				  const char *cmd_name_r, const char *cmd_desc_r, int min_range, int max_range, int default_value,
				  int starter, int spring);

/**
 * @brief Refreshes all registered switches.
 */
void sw_ref(void);

/**
 * @brief Destroys all registered switches.
 */
void sw_destroy(void);

#endif //LIBSWITCH_H
