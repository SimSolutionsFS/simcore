#include "xpdraw/windows.h"

#include <stdlib.h>
#include <XPLMDisplay.h>

// "Dummy" callbacks. Adapted from https://developer.x-plane.com/code-sample/hello-world-sdk-3/
void base_draw(XPLMWindowID in_window_id, void *in_refcon) {}

int base_mouse(XPLMWindowID in_window_id, int x, int y, int is_down, void *in_refcon) {
	return 0;
}

XPLMCursorStatus base_cursor(XPLMWindowID in_window_id, int x, int y, void *in_refcon) {
	return xplm_CursorDefault;
}

int base_wheel(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void *in_refcon) {
	return 0;
}

void base_key(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void *in_refcon,
			  int losing_focus) {}

// Real functions
void xpd_win_new(xpd_win_t *inWindow, int width, int height) {
	inWindow->width = width;
	inWindow->height = height;
	inWindow->buffer = malloc(width * height * 4 * sizeof(unsigned char));

	inWindow->drawFunc = base_draw;
	inWindow->clickFunc = base_mouse;
	inWindow->wheelFunc = base_wheel;
	inWindow->keyFunc = base_key;
	inWindow->cursorFunc = base_cursor;
}

void xpd_win_set_draw_cb(xpd_win_t *inWindow, XPLMDrawWindow_f new_cb) {
	inWindow->drawFunc = new_cb;
}

void xpd_win_set_click_cb(xpd_win_t *inWindow, XPLMHandleMouseClick_f new_cb) {
	inWindow->clickFunc = new_cb;
}

void xpd_win_set_cursor_cb(xpd_win_t *inWindow, XPLMHandleCursor_f new_cb) {
	inWindow->cursorFunc = new_cb;
}

void xpd_win_set_wheel_cb(xpd_win_t *inWindow, XPLMHandleMouseWheel_f new_cb) {
	inWindow->wheelFunc = new_cb;
}

void xpd_win_set_key_cb(xpd_win_t *inWindow, XPLMHandleKey_f new_cb) {
	inWindow->keyFunc = new_cb;
}

void xpd_win_create(xpd_win_t *inWindow, const char *title) {
	xpd_win_create2(inWindow, title, 50, 150);
}

void xpd_win_create2(xpd_win_t *inWindow, const char *title, int winLeft, int winDown) {
	inWindow->params.structSize = sizeof(inWindow->params);
	inWindow->params.visible = 1;
	inWindow->params.drawWindowFunc = inWindow->drawFunc;
	inWindow->params.handleMouseClickFunc = inWindow->clickFunc;
	inWindow->params.handleRightClickFunc = inWindow->clickFunc;
	inWindow->params.handleMouseWheelFunc = inWindow->wheelFunc;
	inWindow->params.handleKeyFunc = inWindow->keyFunc;
	inWindow->params.handleCursorFunc = inWindow->cursorFunc;
	inWindow->params.refcon = NULL;
	inWindow->params.layer = xplm_WindowLayerFloatingWindows;
	inWindow->params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

	int left, bottom, right, top;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	inWindow->params.left = left + winLeft;
	inWindow->params.bottom = bottom + winDown;
	inWindow->params.right = inWindow->params.left + inWindow->width - 20;
	inWindow->params.top = inWindow->params.bottom + inWindow->height - 20;

#ifdef XPLM440
	inWindow->params.windowContentType = xplm_WindowContentTypePanelGraphics;
#endif

	inWindow->windowID = XPLMCreateWindowEx(&inWindow->params);

	XPLMSetWindowPositioningMode(inWindow->windowID, xplm_WindowPositionFree, -1);
	XPLMSetWindowTitle(inWindow->windowID, title);
}

void xpd_win_resize_lims(const xpd_win_t *inWindow, int minWidth, int minHeight, int maxWidth, int maxHeight) {
	XPLMSetWindowResizingLimits(inWindow->windowID, minWidth - 20, minHeight - 20, maxWidth - 20, maxHeight - 20);
}
