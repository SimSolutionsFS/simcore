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
#ifndef XPDRAW2_H
#define XPDRAW2_H
#include <stdlib.h>

#include "tools.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XPLMPG1
#include <XPLMPanelGraphics.h>

#define XPD_COLOR_WHITE XPLMMakeColor(1.0, 1.0, 1.0, 1.0);
#define XPD_COLOR_BLACK XPLMMakeColor(0, 0, 0, 0);
#define XPD_RGB(r, g, b) XPLMMakeColor(r/255.0f, g/255.0f, b/255.0f, 1);
#define XPD_RGBA(r, g, b, a) XPLMMakeColor(r/255.0f, g/255.0f, b/255.0f, a);

typedef XPLMTextureAtlasRef xpd_texture_t;
typedef uint32_t xpd_color_t;

/**
 * @brief Sets the position of the 'anchor'. All future calls will be relative to this location if defined.
 *
 * @param newAnchor_x Lateral position of the anchor
 * @param newAnchor_y Vertical position of the anchor
 */
void xpd_set_anchor(float newAnchor_x, float newAnchor_y);

/**
 * @brief Draw a triangle with the specified points
 *
 * @param x Left point of the triangle
 * @param y Bottom point of the triangle
 * @param width Width of the triangle
 * @param height Height of the triangle
 * @param color Color of the triangle
 */
void xpd_draw_triangle(float x, float y, float width, float height, xpd_color_t color);

/**
 * @brief TODO: Draw a circle with the specified parameters
 *
 * @param left Left side of the circle
 * @param bottom Bottom side of the circle
 * @param r Radius of the circle (NOT diameter!)
 * @param color Color of the circle
 */
//void xpd_draw_circle(int left, int bottom, int r, xpd_color_t color);

/**
 * @brief Draw a rectangle with the specified parameters
 *
 * @param left X position of the rectangle
 * @param bottom Y position of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 */
void xpd_draw_rect(float left, float bottom, float width, float height, xpd_color_t color);

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
void xpd_draw_rect2(float left, float bottom, float width, float height, xpd_color_t color, float borderSize,
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
void xpd_draw_quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, xpd_color_t color);

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
void xpd_draw_line(float start_x, float start_y, float end_x, float end_y, xpd_color_t color, float thickness);

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
void xpd_draw_texture2(xpd_texture_t *texture, float left, float bottom, float width, float height, xpd_color_t color,
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
void xpd_draw_texture(xpd_texture_t *texture, float left, float bottom, float width, float height, xpd_color_t color);

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
void xpd_draw_rotated_texture(xpd_texture_t *texture, float angle, float left, float bottom, float width, float height, float rx,
							  float ry, xpd_color_t color);

/**
 * @brief Load a texture
 *
 * @param texture Pointer to the xpdraw texture
 * @param filename File path to load
 */
void xpd_load_texture(xpd_texture_t *texture, const char *filename);

/**
 * @brief Load a texture, but combine root and filename to create the filepath
 *
 * @param texture Pointer to the texture
 * @param root First half of filepath
 * @param filename Second half of filepath
 */
inline void xpd_load_texture2(xpd_texture_t *texture, const char *root, const char *filename) {
	char *txt_pth_tmp = xpd_tools_constr(root, filename);
	xpd_load_texture(texture, txt_pth_tmp);
	free(txt_pth_tmp);
}

#ifdef __cplusplus
}
#endif

#endif
