/*
 * xpdraw.h - OpenGL rendering functions
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
#ifndef XPDRAW_H
#define XPDRAW_H
#include <stdlib.h>

#include "tools.h"

#if LIN
#include <GL/gl.h>
#elif __MINGW32__
#include <GL/gl.h>
#elif __GNUC__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define XPD_COLOR_WHITE (xpd_color_t){ 1, 1, 1, 1 }
#define XPD_COLOR_BLACK (xpd_color_t){ 0, 0, 0, 1 }
#define XPD_RGB(r, g, b) (xpd_color_t){ r/255.0f, g/255.0f, b/255.0f, 1 }
#define XPD_RGBA(r, g, b, a) (xpd_color_t){ r/255.0f, g/255.0f, b/255.0f, a }

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	GLuint gl_texture;
	int width;
	int height;
} xpd_texture_t;

typedef struct {
	float red;
	float green;
	float blue;
	float alpha;
} xpd_color_t;

static const xpd_texture_t xpd_texture_empty = {
	0,
	0,
	0
};

/**
 * @brief Loads an OpenGL buffer and converts it into a xpdraw texture
 *
 * @param texture Pointer to the xpdraw texture
 * @param buffer Buffer to load
 * @param width Width of buffer
 * @param height Height of buffer
 * @param format Format of buffer
 */
void xpd_load_buffer(xpd_texture_t *texture, void *buffer, unsigned int width, unsigned int height, GLint format);

/**
 * @brief Sets the position of the 'anchor'. All future calls will be relative to this location if defined.
 *
 * @param newAnchor_x Lateral position of the anchor
 * @param newAnchor_y Vertical position of the anchor
 */
void xpd_set_anchor(int newAnchor_x, int newAnchor_y);

/**
 * @brief Draw a triangle with the specified points
 *
 * @param x Left point of the triangle
 * @param y Bottom point of the triangle
 * @param width Width of the triangle
 * @param height Height of the triangle
 * @param color Color of the triangle
 */
void xpd_draw_triangle(int x, int y, int width, int height, xpd_color_t color);

/**
 * @brief Draw a rectangle with the specified parameters
 *
 * @param left X position of the rectangle
 * @param bottom Y position of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 */
void xpd_draw_rect(int left, int bottom, int width, int height, xpd_color_t color);

/**
 * @brief Draw a rectangle with the specified parameters
 *
 * @param left X position of the rectangle
 * @param bottom Y position of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 * @param borderSize Border size of the rectangle if needed; set to -1 to not show
 * @param borderColor Color of the rectangle border (if shown)
 */
void xpd_draw_rect2(int left, int bottom, int width, int height, xpd_color_t color, int borderSize,
					xpd_color_t borderColor);

/**
 * @brief Draw a quad with the specified points
 *
 * @param x1 Lateral position of the first point
 * @param y1 Vertical position of the first point
 * @param x2 Lateral position of the second point
 * @param y2 Vertical position of the second point
 * @param x3 Lateral position of the third point
 * @param y3 Vertical position of the third point
 * @param x4 Lateral position of the fourth point
 * @param y4 Vertical position of the fourth point
 * @param color Color of the quad
 */
void xpd_draw_quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, xpd_color_t color);

/**
 * @brief Draw a line
 *
 * @param start_x X position of the starting point
 * @param start_y Y position of the starting point
 * @param end_x X position of the ending point
 * @param end_y Y position of the ending point
 * @param color Color of the line
 * @param thickness Thickness of the line
 */
void xpd_draw_line(int start_x, int start_y, int end_x, int end_y, xpd_color_t color, float thickness);

/**
 * @brief Draw a mirrored version of a texture; used by fonts
 *
 * @param texture Texture to draw
 * @param left X position of the texture
 * @param bottom Y position of the texture
 * @param width Width of the drawn texture; set to 0 to use the texture's width
 * @param height Height of the drawn texture; set to 0 to use the texture's height
 * @param color Color of the texture; defaults to white
 * @param flipped Flips the texture vertically, if true
 */
void xpd_draw_texture2(xpd_texture_t *texture, int left, int bottom, int width, int height, xpd_color_t color,
					   int flipped);

/**
 * @brief Draw a mirrored version of a texture; used by fonts
 *
 * @param texture Texture to draw
 * @param left X position of the texture
 * @param bottom Y position of the texture
 * @param width Width of the drawn texture; set to 0 to use the texture's width
 * @param height Height of the drawn texture; set to 0 to use the texture's height
 * @param color Color of the texture; defaults to white
 */
void xpd_draw_texture(xpd_texture_t *texture, int left, int bottom, int width, int height, xpd_color_t color);

/**
 * @brief Draw a rotated texture
 *
 * @param texture Texture to draw
 * @param angle Angle to draw the texture at
 * @param left X position of the texture
 * @param bottom Y position of the texture
 * @param width Width of the drawn texture; defaults to the texture's width
 * @param height Height of the drawn texture; defaults to the texture's height
 * @param rx X anchor to rotate around relative to the texture
 * @param ry Y anchor to rotate around relative to the texture
 * @param color Color of the texture
 */
void xpd_draw_rotated_texture(xpd_texture_t *texture, double angle, int left, int bottom, int width, int height, int rx,
							  int ry, xpd_color_t color);

/**
 * @brief Load a texture
 *
 * @param texture Pointer to the xpdraw texture
 * @param fmt File path to load
 */
void xpd_load_texture(xpd_texture_t *texture, const char *fmt, ...) __attribute__ ((format(printf, 2, 3)));

/**
 * @brief Load a texture, but combine root and filename to create the filepath
 *
 * @param texture Pointer to the texture
 * @param root First half of filepath
 * @param filename Second half of filepath
 */
[[deprecated]]
inline void xpd_load_texture2(xpd_texture_t *texture, const char *root, const char *filename) {
	char *txt_pth_tmp = xpd_tools_constr(root, filename);
	xpd_load_texture(texture, txt_pth_tmp);
	free(txt_pth_tmp);
}

#ifdef __cplusplus
}
#endif

#endif
