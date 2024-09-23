/*
    PowerLEDColorU
    By Solace D. Ellery
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <optional>

#include <wups.h>
#include <wut.h>
#include <whb/log.h>
#include <whb/log_module.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>

#include <notifications/notifications.h>
#include <utils/logger.h>

#include <coreinit/kernel.h>
#include <coreinit/filesystem.h>
#include <coreinit/title.h>
#include <sysapp/title.h>
#include <mocha/mocha.h>

#include <string>
#include <cstring>
#include <string_view>
#include <cstdint>

#include "config.h"

#include "Notification.h"

/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("PowerLEDColorU");
WUPS_PLUGIN_DESCRIPTION("Allows you to change the Wii U's LED Color, located on the Power Button, while the console is turned on");
WUPS_PLUGIN_VERSION("v1.0.0");
WUPS_PLUGIN_AUTHOR("Solace D. Ellery");
WUPS_PLUGIN_LICENSE("MIT");

WUPS_USE_STORAGE("PowerLEDColorU");
WUPS_USE_WUT_DEVOPTAB();

INITIALIZE_PLUGIN() {
    // Init logging
    if (!WHBLogModuleInit()) {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    Config::Init();

    auto res = Mocha_InitLibrary();

    if (res != MOCHA_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Mocha init failed with code %d!", res);
        return;
    }

    if (NotificationModule_InitLibrary() != NOTIFICATION_MODULE_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("NotificationModule_InitLibrary failed");
    }

    ShowNotification("The PowerLEDColorU plugin has started!");
}

DEINITIALIZE_PLUGIN() {
    Mocha_DeInitLibrary();

    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
}

ON_APPLICATION_START() {
    uint64_t current_title_id = OSGetTitleID();
    uint64_t wiiu_menu_tid = _SYSGetSystemApplicationTitleId(SYSTEM_APP_ID_WII_U_MENU);

    bool is_wiiu_menu = (current_title_id == wiiu_menu_tid);

    if (is_wiiu_menu)
    {
        
    }
}

ON_APPLICATION_ENDS() {

}
