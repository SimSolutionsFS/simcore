/*
 * win.h - X-Plane window handler
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
#ifndef SIMCORE_WIN_H
#define SIMCORE_WIN_H

#include <XPLMDisplay.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef XPLM440
typedef struct {
	XPLMCreateWindow_t params;
	XPLMWindowID id;
	int texNum;
	unsigned char *buffer;
	int width;
	int height;
	XPLMDrawWindow_f draw_func;
	XPLMHandleMouseClick_f click_func;
	XPLMHandleMouseWheel_f wheel_func;
	XPLMHandleKey_f key_func;
	XPLMHandleCursor_f cursor_func;
} sc_win_t;

/**
 * @brief Create a new window
 *
 * @param in_win Window to create
 * @param width Width of the window's framebuffer
 * @param height Height of the window's framebuffer
 */
void sc_win_new(sc_win_t *in_win, int width, int height);

/**
 * @brief Register callbacks for a window
 *
 * @param in_win Window to modify
 * @param draw_cb New draw callback
 * @param click_cb New mouse click callback
 * @param cursor_cb New mouse cursor callback
 * @param wheel_cb New mouse scroll callback
 * @param key_cb New keyboard press callback
 */
void sc_win_set_cb(sc_win_t *in_win, XPLMDrawWindow_f draw_cb, XPLMHandleMouseClick_f click_cb,
				   XPLMHandleCursor_f cursor_cb, XPLMHandleMouseWheel_f wheel_cb, XPLMHandleKey_f key_cb);

/**
 * @brief Display a window created with xpd_win_new. Panel graphics *must* be used.
 *
 * @param in_win Window to start rendering
 * @param title Title of the window
 */
void sc_win_create(sc_win_t *in_win, const char *title);

/**
 * @brief Display a window created with xpd_win_new. Panel graphics *must* be used.
 *
 * @param in_win Window to start rendering
 * @param title Title of the window
 * @param win_left Position of the left edge of the window
 * @param win_down Position of the bottom edge of the window
 */
void sc_win_create2(sc_win_t *in_win, const char *title, int win_left, int win_down);

/**
 * @brief Set the minimum & maximum sizes of a window
 *
 * @param in_win Window to update
 * @param min_w New minimum width of the window
 * @param min_h New minimum height of the window
 * @param max_w New maximum width of the window
 * @param max_h New maximum height of the window
 */
void sc_win_set_size(const sc_win_t *in_win, int min_w, int min_h, int max_w, int max_h);
#endif

#ifdef __cplusplus
}
#endif

#endif
