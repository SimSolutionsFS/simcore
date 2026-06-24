/*
 * fonts.h - Font handler & renderer
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
#ifndef XPDRAW_FONTS2_H
#define XPDRAW_FONTS2_H

#include "xpdraw2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	XPD_ALIGN_L = xplm_JustLeft,
	XPD_ALIGN_C = xplm_JustCenter,
	XPD_ALIGN_R = xplm_JustRight
} xpd_text_align_t;

typedef struct {
	XPLMFontHandle face;
	int size;
} xpd_font_face_t;

/**
 * @brief Load a new font
 *
 * @param font Pointer to the font we are loading
 * @param path File path to load from
 * @param size Size of font face to use
 */
void xpd_font_load(xpd_font_face_t *font, const char *path, int size);

/**
 *	@brief Load a new font, but combine root and filename to create the filepath
 *
 * @param font Pointer to the font face
 * @param root First half of filepath
 * @param filename Second half of filepath
 * @param size Size of the font face
 */
inline void xpd_font_load2(xpd_font_face_t *font, const char *root, const char *filename, const int size) {
	char *fnt_pth_tmp = xpd_tools_constr(root, filename);
	xpd_font_load(font, fnt_pth_tmp, size);
	free(fnt_pth_tmp);
}

/**
 * @brief Returns the length of a string.
 *
 * @param font Font to use
 * @param text Text to get the length of
 * @return int
 */
int xpd_text_length(xpd_font_face_t *font, const char *text);

/**
 * @brief Function to draw text
 *
 * @param font Font to use
 * @param text Text to render
 * @param x Lateral position to draw at relative to anchor
 * @param y Vertical position to draw at relative to anchor
 * @param align Alignment of the text relative to x
 * @param color Color of the text; defaults to white
 */
void xpd_text_draw(xpd_font_face_t *font, const char *text, int x, int y, xpd_text_align_t align,
				   xpd_color_t color);

#ifdef __cplusplus
}
#endif

#endif
