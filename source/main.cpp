/*
    PowerLEDColorU
    Created by SpencerlyEverly
*/

#include <wups.h>
#include <whb/log.h>
#include <whb/log_module.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>
#include <string.h>

#include "config.h"

WUPS_PLUGIN_NAME("PowerLEDColorU");
WUPS_PLUGIN_DESCRIPTION("Allows you to change the Wii U Power LED Color, toggle blinking, and even turn off the LED while the console is turned on");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("SpencerlyEverly");
WUPS_PLUGIN_LICENSE("MIT");

WUPS_USE_STORAGE("powerLedColorU");

INITIALIZE_PLUGIN() {
    // Init logging
    if (!WHBLogModuleInit()) {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    ShowNotification("The PowerLEDColorU process has started!");
    Config::Init();
}

DEINITIALIZE_PLUGIN() {
    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
}
