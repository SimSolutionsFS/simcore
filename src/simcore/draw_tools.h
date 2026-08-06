/*
 * draw_tools.h - Simplified functions for X-Plane panel graphics, similar to the legacy xpdraw.h
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
#ifndef DRAW_TOOLS_H
#define DRAW_TOOLS_H

#ifdef XPLM440
#ifdef __cplusplus
extern "C" {


#endif

#include <XPLMPanelGraphics.h>

#define SC_RGB(r, g, b) XPLMMakeColor(r/255.0f, g/255.0f, b/255.0f, 1);
#define SC_RGBA(r, g, b, a) XPLMMakeColor(r/255.0f, g/255.0f, b/255.0f, a);


/**
 * @brief Draw a triangle with the specified points
 *
 * @param x Left point of the triangle
 * @param y Bottom point of the triangle
 * @param width Width of the triangle
 * @param height Height of the triangle
 * @param color Color of the triangle
 */
void sc_draw_triangle(float x, float y, float width, float height, uint32_t color);

/**
 * @brief TODO: Draw a circle with the specified parameters
 *
 * @param left Left side of the circle
 * @param bottom Bottom side of the circle
 * @param r Radius of the circle (NOT diameter!)
 * @param color Color of the circle
 */
void sc_draw_circle(int left, int bottom, int r, uint32_t color);

/**
 * @brief Draw a rectangle with the specified parameters
 *
 * @param left X position of the rectangle
 * @param bottom Y position of the rectangle
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 */
void sc_draw_rect(float left, float bottom, float width, float height, uint32_t color);

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
void sc_draw_rect2(float left, float bottom, float width, float height, uint32_t color, float borderSize,
				   uint32_t borderColor);

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
void sc_draw_line(float start_x, float start_y, float end_x, float end_y, uint32_t color, float thickness);

#ifdef __cplusplus
}
#endif
#endif

#endif
