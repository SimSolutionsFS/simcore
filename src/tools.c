#include "xpdraw/tools.h"

#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>

int xp_ver = -1;
char xp_path[512];
char plugin_path[512];

char *xpd_tools_constr(const char *str1, const char *str2) {
	char *tmp_str = malloc(sizeof(char) * 512);
	sprintf(tmp_str, "%s%s", str1, str2);
	return tmp_str;
}

char *xpd_tools_plugin_fp() {
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

	if (strlen(plugin_path) == 0) {
		XPLMGetPluginInfo(XPLMGetMyID(), NULL, plugin_path, NULL, NULL);
		strcpy(plugin_path, dirname(dirname(plugin_path)));
		strncat(plugin_path, "/", sizeof(plugin_path) - strlen(plugin_path) - 1);
	}

	return plugin_path;
}

char *xpd_tools_xp_fp() {
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);

	if (strlen(xp_path) == 0) {
		XPLMGetSystemPath(xp_path);
	}

	return xp_path;
}

int xpd_tools_xp_ver() {
	if (xp_ver == -1) {
		XPLMGetVersions(&xp_ver, NULL, NULL);
		xp_ver = xp_ver / 1000;
	}
	return xp_ver;
}
