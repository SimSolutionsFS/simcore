#include "xpdraw/xpdraw.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <acfutils/assert.h>

int anchor_x = 0;
int anchor_y = 0;

void xpd_load_buffer(xpd_texture_t *texture, void *buffer, unsigned int width, unsigned int height, GLint format) {
	texture->width = (int)width;
	texture->height = (int)height;

	glGenTextures(1, &texture->gl_texture);
	glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void xpd_set_anchor(int newAnchor_x, int newAnchor_y) {
	anchor_x = newAnchor_x;
	anchor_y = newAnchor_y;
}

void xpd_draw_triangle(int x, int y, int width, int height, xpd_color_t color) {
	glColor4f(color.red, color.green, color.blue, color.alpha);

	// Handle flipped triangles
	if (height < 0) {
		y = y - height;

		glBegin(GL_TRIANGLES);
		glVertex2i(x + anchor_x, y + anchor_y);
		glVertex2i(x + width + anchor_x, y + anchor_y);
		glVertex2i(x + (width / 2) + anchor_x, y + height + anchor_y);
		glEnd();
	}
	else {
		glBegin(GL_TRIANGLES);
		glVertex2i(x + anchor_x, y + anchor_y);
		glVertex2i(x + (width / 2) + anchor_x, y + height + anchor_y);
		glVertex2i(x + width + anchor_x, y + anchor_y);
		glEnd();
	}
}

void xpd_draw_rect(int left, int bottom, int width, int height, xpd_color_t color) {
	glColor4f(color.red, color.green, color.blue, color.alpha);

	const int x1 = anchor_x + left;
	const int y1 = anchor_y + bottom;
	const int x2 = x1 + width;
	const int y2 = y1 + height;
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
}

void xpd_draw_rect2(int left, int bottom, int width, int height, xpd_color_t color, int borderSize,
					xpd_color_t borderColor) {
	xpd_draw_rect(left, bottom, width, height, color);
	xpd_draw_rect(left, bottom, borderSize, height, borderColor);
	xpd_draw_rect(left + width - borderSize, bottom, borderSize, height, borderColor);
	xpd_draw_rect(left, bottom + height - borderSize, width, borderSize, borderColor);
	xpd_draw_rect(left, bottom, width, borderSize, borderColor);
}

void xpd_draw_quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, xpd_color_t color) {
	glColor4f(color.red, color.green, color.blue, color.alpha);

	glBegin(GL_QUADS);
	glVertex2i(x1 + anchor_x, y1 + anchor_y);
	glVertex2i(x2 + anchor_x, y2 + anchor_y);
	glVertex2i(x3 + anchor_x, y3 + anchor_y);
	glVertex2i(x4 + anchor_x, y4 + anchor_y);
	glEnd();
}

void xpd_draw_line(int start_x, int start_y, int end_x, int end_y, xpd_color_t color, float thickness) {
	glColor4f(color.red, color.green, color.blue, color.alpha);

	const int x1 = start_x + anchor_x;
	const int y1 = start_y + anchor_y;
	const int x2 = end_x + anchor_x;
	const int y2 = end_y + anchor_y;

	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

void xpd_draw_texture2(xpd_texture_t *texture, int left, int bottom, int width, int height, xpd_color_t color,
					   bool flipped) {
	glColor4f(color.red, color.green, color.blue, color.alpha);

	if (width == 0) {
		width = texture->width;
	}
	if (height == 0) {
		height = texture->height;
	}

	const int x1 = anchor_x + left;
	const int y1 = anchor_y + bottom;
	const int x2 = x1 + width;
	const int y2 = y1 + height;

	glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1 - flipped);
	glVertex2i(x1, y1);
	glTexCoord2i(0, 0 + flipped);
	glVertex2i(x1, y2);
	glTexCoord2i(1, 0 + flipped);
	glVertex2i(x2, y2);
	glTexCoord2i(1, 1 - flipped);
	glVertex2i(x2, y1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void xpd_draw_texture(xpd_texture_t *texture, int left, int bottom, int width, int height, xpd_color_t color) {
	xpd_draw_texture2(texture, left, bottom, width, height, color, false);
}

void xpd_draw_rotated_texture(xpd_texture_t *texture, double angle, int left, int bottom, int width, int height, int rx,
							  int ry, xpd_color_t color) {
	glColor4f(color.red, color.green, color.blue, color.alpha);

	rx = left + rx;
	ry = bottom + ry;

	const int x1 = left - rx;
	const int y1 = bottom - ry;
	const int x2 = x1 + width;
	const int y2 = y1 + height;

	glBindTexture(GL_TEXTURE_2D, texture->gl_texture);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslated(rx + anchor_x, ry + anchor_y, 0);
	glRotated(360 - angle, 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 1);
	glVertex2i(x1, y1);
	glTexCoord2i(0, 0);
	glVertex2i(x1, y2);
	glTexCoord2i(1, 0);
	glVertex2i(x2, y2);
	glTexCoord2i(1, 1);
	glVertex2i(x2, y1);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void xpd_load_texture(xpd_texture_t *texture, const char *filename) {
	// Get a buffer from the passed file
	int width, height, nrChannels; // Do we need to store nrChannels?
	unsigned char *texDat = stbi_load(filename, &width, &height, &nrChannels, 4);

	// Load the buffer into a xpd texture
	ASSERT_MSG(texDat != NULL, "Could not load texture at %s!", filename);

	xpd_load_buffer(texture, texDat, width, height, GL_RGBA);
	stbi_image_free(texDat);
}
