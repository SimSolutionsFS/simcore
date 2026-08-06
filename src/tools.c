#include "simcore/tools.h"

#include <libgen.h>
#include <string.h>
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>

static int xp_ver = -1;
static char xp_path[512];
static char plugin_path[512];

char *tools_plugin_fp(void) {
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

	if (strlen(plugin_path) == 0) {
		XPLMGetPluginInfo(XPLMGetMyID(), NULL, plugin_path, NULL, NULL);
		strcpy(plugin_path, dirname(dirname(plugin_path))); // the double "dirname" here is intentional
		strncat(plugin_path, "/", sizeof(plugin_path) - strlen(plugin_path) - 1);
	}

	return plugin_path;
}

char *tools_xp_fp(void) {
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

	if (strlen(xp_path) == 0) {
		XPLMGetSystemPath(xp_path);
	}

	return xp_path;
}

int tools_xp_ver(void) {
	if (xp_ver == -1) {
		XPLMGetVersions(&xp_ver, NULL, NULL);
		xp_ver = xp_ver / 1000;
	}
	return xp_ver;
}
