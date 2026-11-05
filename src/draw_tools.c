#include "simcore/draw_tools.h"

#include <math.h>

void sc_draw_triangle(const float x, float y, const float width, const float height, const uint32_t color) {
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

void sc_draw_circle(const float left, const float bottom, const float r, const uint32_t color) {
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

void sc_draw_rect(const float left, const float bottom, const float width, const float height, const uint32_t color) {
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

void sc_draw_rect2(const float left, const float bottom, const float width, const float height, const uint32_t color,
				   const float border_size, const uint32_t borderColor) {
	sc_draw_rect(left, bottom, width, height, color);
	sc_draw_rect(left, bottom, border_size, height, borderColor);
	sc_draw_rect(left + width - border_size, bottom, border_size, height, borderColor);
	sc_draw_rect(left, bottom + height - border_size, width, border_size, borderColor);
	sc_draw_rect(left, bottom, width, border_size, borderColor);
}

void sc_draw_line(const float start_x, const float start_y, const float end_x, const float end_y, const uint32_t color,
				  const float thickness) {
	XPLMVertex_t vlist[4] = {
		{start_x, start_y},
		{end_x, end_y},
	};
	XPLMLinesWithWidth(color, thickness, vlist, 2);
}
