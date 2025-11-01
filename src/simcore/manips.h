#ifndef LIBSWITCH_H
#define LIBSWITCH_H

#include <stdbool.h>

/**
 * @brief Switch 'type'. Not a pointer, but rather an index of all loaded switches.
 */
typedef int switch_t;

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
switch_t sw_init(const char *dr_name, const char *dr_anim_name, const char *cmd_name, const char *cmd_desc, int spring);

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
 *
 * @return An ID for the switch.
 */
switch_t sw_init2(const char *dr_name, const char *dr_anim_name, const char *cmd_name_l, const char *cmd_desc_l,
					   const char *cmd_name_r, const char *cmd_desc_r, const int min_range, const int max_range,
					   const int default_value, const bool starter);

/**
 * @brief Gets the state of a switch.
 *
 * @param inRefcon Switch to fetch
 * @return Current position of the switch (1 is usually on and 2 is usually off)
 */
int sw_get_state(void *inRefcon);

/**
 * @brief Writes the state of a switch.
 *
 * @param inRefcon Switch to modify
 * @param inValue New value
 * @return New position of the switch (1 is usually on and 2 is usually off)
 */
void sw_write_state(void *inRefcon, int inValue);

/**
 * @brief Refreshes all registered switches.
 */
void sw_ref(void);

/**
 * @brief Destroys all registered switches.
 */
void sw_destroy();

#endif //LIBSWITCH_H
