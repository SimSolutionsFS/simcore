#ifndef SIMCORE_PERSIST_H
#define SIMCORE_PERSIST_H

#include "simcore/manips.h"

/**
 * @brief Initializes the persistence system.
 *
 * @param in_sync_sw Pointer to the list of switches to track
 * @param in_folder_pth Use a format like "/DA40/"
 */
void persist_init(sw_t **in_sync_sw, const char *in_folder_pth);

/**
 * @brief Starts the persistence system on aircraft load.
 */
void persist_on_load(void);

/**
 * @brief Unregisters the flight loop registered in persist_on_load().
 */
void persist_stop(void);

#endif
