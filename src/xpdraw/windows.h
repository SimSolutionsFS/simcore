/*
 * windows.h - X-Plane window handler
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
#ifndef XPDRAW_WINDOWS_H
#define XPDRAW_WINDOWS_H

#include <XPLMDisplay.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	XPLMCreateWindow_t params;
	XPLMWindowID windowID;
	int texNum;
	unsigned char *buffer;
	int width;
	int height;
	XPLMDrawWindow_f drawFunc;
	XPLMHandleMouseClick_f clickFunc;
	XPLMHandleMouseWheel_f wheelFunc;
	XPLMHandleKey_f keyFunc;
	XPLMHandleCursor_f cursorFunc;
} xpd_win_t;

// Dummy callbacks adapted from https://developer.x-plane.com/code-sample/hello-world-sdk-3/
/**
 * @brief DO NOT USE
 */
void base_draw(XPLMWindowID in_window_id, void *in_refcon);

/**
 * @brief DO NOT USE
 */
int base_mouse(XPLMWindowID in_window_id, int x, int y, int is_down, void *in_refcon);

/**
 * @brief DO NOT USE
 */
XPLMCursorStatus base_cursor(XPLMWindowID in_window_id, int x, int y, void *in_refcon);

/**
 * @brief DO NOT USE
 */
int base_wheel(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void *in_refcon);

/**
 * @brief DO NOT USE
 */
void base_key(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void *in_refcon,
			  int losing_focus);

/**
 * @brief Create a new window
 *
 * @param inWindow Window to create
 * @param width Width of the window's framebuffer
 * @param height Height of the window's framebuffer
 */
void xpd_win_new(xpd_win_t *inWindow, int width, int height);

/**
 * @brief Register a callback to render the window
 *
 * @param inWindow Window to update
 * @param new_cb New callback
 */
void xpd_win_set_draw_cb(xpd_win_t *inWindow, XPLMDrawWindow_f new_cb);

/**
 * @brief Register a callback to handle clicks in the window
 *
 * @param inWindow Window to update
 * @param new_cb New callback
 */
void xpd_win_set_click_cb(xpd_win_t *inWindow, XPLMHandleMouseClick_f new_cb);

/**
 * @brief Register a callback to handle the cursor style (how it looks on the screen)
 *
 * @param inWindow Window to update
 * @param new_cb New callback
 */
void xpd_win_set_cursor_cb(xpd_win_t *inWindow, XPLMHandleCursor_f new_cb);

/**
 * @brief Register a callback to handle scrolling in a window
 *
 * @param inWindow Window to update
 * @param new_cb New callback
 */
void xpd_win_set_wheel_cb(xpd_win_t *inWindow, XPLMHandleMouseWheel_f new_cb);

/**
 * @brief Register a callback to handle keyboard presses in a window
 *
 * @param inWindow
 * @param new_cb
 */
void xpd_win_set_key_cb(xpd_win_t *inWindow, XPLMHandleKey_f new_cb);

/**
 * @brief Display a window created with xpd_win_new
 *
 * @param inWindow Window to start rendering
 * @param title Title of the window
 */
void xpd_win_create(xpd_win_t *inWindow, const char *title);

/**
 * @brief Display a window created with xpd_win_new
 *
 * @param inWindow Window to start rendering
 * @param title Title of the window
 * @param winLeft Position of the left edge of the window
 * @param winDown Position of the bottom edge of the window
 */
void xpd_win_create2(xpd_win_t *inWindow, const char *title, int winLeft, int winDown);

/**
 * @brief Set the minimum & maximum sizes of a window
 *
 * @param inWindow Window to update
 * @param minWidth New minimum width of the window
 * @param minHeight New minimum height of the window
 * @param maxWidth New maximum width of the window
 * @param maxHeight New maximum height of the window
 */
void xpd_win_resize_lims(const xpd_win_t *inWindow, int minWidth, int minHeight, int maxWidth, int maxHeight);

#ifdef __cplusplus
}
#endif

#endif
