#include "xpdraw/fonts2.h"

#include <acfutils/assert.h>

void xpd_font_load(xpd_font_face_t *font, const char *path, const int size) {
	font->size = size;
	font->face = XPLMCreateFont(xplm_CharSetUnicode);

	XPLMFontAddFace(font->face, path);
}

int xpd_text_length(xpd_font_face_t *font, const char *text) {
	return XPLMFontMeasureString(font->face, font->size, text);
}

void xpd_text_draw(xpd_font_face_t *font, const char *text, int x, int y, xpd_text_align_t align,
				   xpd_color_t textColor) {
	ASSERT(font != NULL);

	XPLMFontDrawString(font->face, textColor, font->size, x, y, text, align);
}
