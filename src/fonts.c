#include "xpdraw/fonts.h"

#include <assert.h>

int fonts_init = 0;
FT_Library ft;

void xpd_font_load(xpd_font_face_t *font, const char *fmt, const int size, ...) {
	va_list args;
	va_start(args, fmt);

	char *path = malloc(sizeof(char) * 256);
	vsnprintf(path, 256, fmt, args);
	va_end(args);

	if (fonts_init == 0) {
		FT_Init_FreeType(&ft);
		fonts_init = 1;
	}

	FT_New_Face(ft, path, 0, &font->ftFace);
	assert(font->ftFace != NULL);

	// Tell FreeType what font size we want
	FT_Set_Pixel_Sizes(font->ftFace, 0, (int)(size * 1.5));

	// Load data for each available character
	for (int i = 0; i <= XPD_CHAR_MAX - 1; i++) {
		FT_Load_Char(font->ftFace, i + CHAR_MIN, FT_LOAD_RENDER);

		font->letters[i].letter = (char)(i + CHAR_MIN);
		font->letters[i].metrics = font->ftFace->glyph->metrics;

		xpd_load_buffer(&font->letters[i].bitmap, font->ftFace->glyph->bitmap.buffer, font->ftFace->glyph->bitmap.width,
						font->ftFace->glyph->bitmap.rows, GL_ALPHA);
	}

	FT_Done_Face(font->ftFace);
	free(path);
}

int xpd_text_length(xpd_font_face_t *font, const char *text) {
	int width = 0;

	// Calculate the length of the string before drawing it
	for (int i = 0; i < strlen(text); i++) {
		FT_Glyph_Metrics text_metrics = font->letters[(text[i] - CHAR_MIN)].metrics;
		if (i == strlen(text) - 1) {
			width += (int)((text_metrics.width + text_metrics.horiBearingX) / 64);
		}
		else {
			width += (int)(text_metrics.horiAdvance / 64);
		}
	}

	return width;
}

void xpd_text_draw(xpd_font_face_t *font, const char *text, int x, int y, xpd_text_align_t align,
				   xpd_color_t textColor) {
	assert(font != NULL);

	glColor4f(textColor.red, textColor.green, textColor.blue, textColor.alpha);

	// Handle text alignment
	if (align == XPD_ALIGN_C) {
		x -= xpd_text_length(font, text) / 2;
	}
	else if (align == XPD_ALIGN_R) {
		x -= xpd_text_length(font, text);
	}

	// Draw each character
	for (int i = 0; i < strlen(text); i++) {
		FT_Glyph_Metrics text_metrics = font->letters[(text[i] - CHAR_MIN)].metrics;

		// Calculate offset from the passed y value
		int y_offset = (int)(text_metrics.horiBearingY / 64) - (int)(text_metrics.height / 64);

		// Fetch & draw texture
		xpd_texture_t image = font->letters[(text[i] - CHAR_MIN)].bitmap;
		xpd_draw_texture(&image, x + (int)(text_metrics.horiBearingX / 64), y + y_offset, image.width, image.height,
						 textColor);

		// Advance to the next character
		x += (int)text_metrics.horiAdvance / 64;
	}
}
