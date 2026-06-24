#include "xpdraw/xpdraw2.h"

#include <acfutils/assert.h>

float anchor_x = 0;
float anchor_y = 0;

void xpd_set_anchor(float newAnchor_x, float newAnchor_y) {
	anchor_x = newAnchor_x;
	anchor_y = newAnchor_y;
}

void xpd_draw_triangle(float x, float y, float width, float height, xpd_color_t color) {
	float x1, y1, x2, y2, x3, y3;

	if (height < 0) {
		y = y - height;

		x1 = x + anchor_x;
		y1 = y + anchor_y;
		x2 = x + width + anchor_x;
		y2 = y + anchor_y;
		x3 = x + (width/2) + anchor_x;
		y3 = y + height + anchor_y;
	}
	else {
		x1 = x + anchor_x;
		y1 = y + anchor_y;
		x2 = x + (width / 2) + anchor_x;
		y2 = y + height + anchor_y;
		x3 = x + width + anchor_x;
		y3 = y + anchor_y;
	}

	XPLMVertex_t vlist[3] = {
		{x1, y1},
		{x2, y2},
		{x3, y3}
	};
	XPLMPolygon(color, vlist, 3);
}

void xpd_draw_circle(int left, int bottom, int r, xpd_color_t color) {
	// TODO: Implement this.
}

void xpd_draw_rect(float left, float bottom, float width, float height, xpd_color_t color) {
	const float x1 = anchor_x + left;
	const float y1 = anchor_y + bottom;
	const float x2 = x1 + width;
	const float y2 = y1 + height;

	xpd_draw_quad(x1, y1, x1, y2, x2, y2, x2, y1, color);
}

void xpd_draw_rect2(float left, float bottom, float width, float height, xpd_color_t color, float borderSize,
					xpd_color_t borderColor) {
	xpd_draw_rect(left, bottom, width, height, color);
	xpd_draw_rect(left, bottom, borderSize, height, borderColor);
	xpd_draw_rect(left + width - borderSize, bottom, borderSize, height, borderColor);
	xpd_draw_rect(left, bottom + height - borderSize, width, borderSize, borderColor);
	xpd_draw_rect(left, bottom, width, borderSize, borderColor);
}

void xpd_draw_quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, xpd_color_t color) {
	XPLMVertex_t vlist[4] = {
		{x1, y1},
		{x2, y2},
		{x3, y3},
		{x4, y4}
	};
	XPLMPolygon(color, vlist, 4);
}

void xpd_draw_line(float start_x, float start_y, float end_x, float end_y, xpd_color_t color, float thickness) {
	XPLMVertex_t vlist[2] = { {start_x, start_y}, {end_x, end_y} };
	XPLMLinesWithWidth(color, thickness, vlist, 2);
}

void xpd_draw_texture2(xpd_texture_t *texture, float left, float bottom, float width, float height, xpd_color_t color,
					   int flipped) {
	const float x1 = anchor_x + left;
	float y1 = anchor_y + bottom;
	const float x2 = x1 + width;
	float y2 = y1 + height;

	if (flipped) {
		y2 = anchor_y + bottom;
		y1 = y2 + height;
	}

	XPLMTextureAtlasDrawIn(*texture, 0, color, x1, y2, x2, y1);
}

void xpd_draw_texture(xpd_texture_t *texture, float left, float bottom, float width, float height, xpd_color_t color) {
	xpd_draw_texture2(texture, left, bottom, width, height, color, 0);
}

void xpd_draw_rotated_texture(xpd_texture_t *texture, float angle, float left, float bottom, float width, float height, float rx,
							  float ry, xpd_color_t color) {
	XPLMTextureAtlasDrawScaled(*texture, 0, color, left, bottom, rx, ry, 1.0f, 1.0f, angle);
}

void xpd_load_texture(xpd_texture_t *texture, const char *filename) {
	XPLMTextureAtlasRef new_tex = XPLMCreateTextureAtlas();
	XPLMTextureAtlasAddImageFile(new_tex, filename);
	XPLMTextureAtlasBake(new_tex);

	texture = &new_tex;
}
