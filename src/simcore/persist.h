/*
 * persist.h - Basic persistence system for X-Plane aircraft
 * Copyright 2026 Ian Ward
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#ifndef SIMCORE_PERSIST_H
#define SIMCORE_PERSIST_H

#include "simcore/manips.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the persistence system.
 *
 * @param in_folder_pth Use a format like "/DA40/"
 */
void persist_init(const char *in_folder_pth);

/**
 * @brief Starts the persistence system on aircraft load.
 */
void persist_on_load(void);

/**
 * @brief Unregisters the flight loop registered in persist_on_load().
 */
void persist_stop(void);

#ifdef __cplusplus
}
#endif

#endif
