#ifndef SIMCORE_PERSIST_H
#define SIMCORE_PERSIST_H

/**
 * @brief Initializes the persistence system.
 *
 * @param persist_drs_i List of datarefs to save, as ints
 * @param persist_drs_f List of datarefs to save, as floats
 * @param persist_enabled_in Pointer to an int of the systems state
 */
void persist_init(char *persist_drs_i[64], char *persist_drs_f[64], int *persist_enabled_in);

/**
 * @brief Starts the persistence system on aircraft load.
 */
void persist_on_load(void);

/**
 * @brief Unregisters the flight loop registered in persist_on_load().
 */
void persist_stop(void);

#endif
