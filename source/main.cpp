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

#include "globals.h"
#include "config.h"

#include "Notification.h"

// Reset the global variables
Globals_Startup();

/*
    Set the most-important plugin settings.

    - Name
    - Description
    - Version
    - Author
    - License

    These are needed, otherwise this plugin will fail
*/
WUPS_PLUGIN_NAME(GlobalVarsFuncs.PluginName);
WUPS_PLUGIN_DESCRIPTION(GlobalVarsFuncs.PluginDescription);
WUPS_PLUGIN_VERSION(GlobalVarsFuncs.PluginVersionFull);
WUPS_PLUGIN_AUTHOR(GlobalVarsFuncs.PluginAuthor);
WUPS_PLUGIN_LICENSE("MIT");

// Set to use a config file
WUPS_USE_STORAGE(GlobalVarsFuncs.PluginName);

// Used for debug logs
WUPS_USE_WUT_DEVOPTAB();

// Runs when the plugin first loads.
INITIALIZE_PLUGIN()
{
    // Initalize the logging system
    if (!WHBLogModuleInit())
    {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    // Start the config
    Config::Init();

    // Start Mocha
    auto res = Mocha_InitLibrary();

    // If Mocha fails, stop the plugin
    if (res != MOCHA_RESULT_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Mocha init failed with code %d!", res);
        return;
    }

    // If the Notification module fails, keep going, but mark that it failed
    if (NotificationModule_InitLibrary() != NOTIFICATION_MODULE_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("NotificationModule_InitLibrary failed");
    }

    // Notify that the plugin has officially started and is active, if on the Wii U menu
    if(GlobalVarsFuncs.isOnWiiUMenu())
    {
        ShowNotification(GlobalVarsFuncs.PluginName + " is active and has started!");
    }
}

// Runs when the plugin is about to close.
DEINITIALIZE_PLUGIN()
{
    // Close Mocha
    Mocha_DeInitLibrary();

    // Close logging any further
    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
}

// Runs when any Wii U title starts.
ON_APPLICATION_START()
{
    
}

// Runs when any Wii U title closes.
ON_APPLICATION_ENDS() {

}

// The main plugin code.
int main()
{
    // We'll put in this part as the loop, so this runs on every frame this plugin is on.
    while(1)
    {
        // Since the LED color does not change after setting the config for the plugin, make sure it changes on every frame
        if (!GlobalVarsFuncs.PluginConfig_EnableLED)
        {
            GlobalVarsFuncs.setPowerLEDState(0x00);
        }
        else
        {
            GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfig_ColorValue);
        }
    }

    return 0;
}
