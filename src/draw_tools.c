#include "simcore/draw_tools.h"

#include <math.h>

void sc_draw_triangle(float x, float y, float width, float height, uint32_t color) {
	float x1, y1, x2, y2, x3, y3;

	if (height < 0) {
		y = y - height;

		x1 = x;
		y1 = y;
		x2 = x + width;
		y2 = y;
		x3 = x + (width / 2);
		y3 = y + height;
	}
	else {
		x1 = x;
		y1 = y;
		x2 = x + (width / 2);
		y2 = y + height;
		x3 = x + width;
		y3 = y;
	}

	XPLMVertex_t vlist[3] = {
		{x1, y1},
		{x2, y2},
		{x3, y3}
	};
	XPLMPolygon(color, vlist, 3);
}

void sc_draw_circle(float left, float bottom, float r, uint32_t color) {
	XPLMVertex_t array[36];

	for (int i = 0; i < 36; i++) {
		double ang = (i * 10) * (M_PI / 180);

		XPLMVertex_t new = {
			(float)(left + r + (cos(ang) * r)),
			(float)(bottom + r + (sin(ang) * r))
		};

		array[i] = new;
	}

	XPLMPolygon(color, array, 36);
}

void sc_draw_rect(float left, float bottom, float width, float height, uint32_t color) {
	const float x1 = left;
	const float y1 = bottom;
	const float x2 = x1 + width;
	const float y2 = y1 + height;

	XPLMVertex_t vlist[4] = {
		{x1, y1},
		{x1, y2},
		{x2, y2},
		{x2, y1}
	};
	XPLMPolygon(color, vlist, 4);
}

void sc_draw_rect2(float left, float bottom, float width, float height, uint32_t color, float borderSize,
				   uint32_t borderColor) {
	sc_draw_rect(left, bottom, width, height, color);
	sc_draw_rect(left, bottom, borderSize, height, borderColor);
	sc_draw_rect(left + width - borderSize, bottom, borderSize, height, borderColor);
	sc_draw_rect(left, bottom + height - borderSize, width, borderSize, borderColor);
	sc_draw_rect(left, bottom, width, borderSize, borderColor);
}

void sc_draw_line(float start_x, float start_y, float end_x, float end_y, uint32_t color, float thickness) {
	XPLMVertex_t vlist[4] = {
		{start_x, start_y},
		{end_x, end_y},
	};
	XPLMLinesWithWidth(color, thickness, vlist, 2);
}