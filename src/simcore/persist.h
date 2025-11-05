#ifndef DA40_PERSIST_H
#define DA40_PERSIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Starts the persistence system once the aircraft & livery is loaded.
 *
 * @param dist_lim_in Distance, in nautical miles, that the last state has to be within for the state to automatically be loaded.
 * @param persist_enabled_in Pointer to the persistence system state.
 */
void persist_init(float dist_lim_in, int *persist_enabled_in);

/**
 * @brief Unregisters the flight loop registered in persist_init()
 */
void persist_stop(void);

#ifdef __cplusplus
}
#endif

#endif