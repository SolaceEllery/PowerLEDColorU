/*
    PowerLEDColorU
    By Solace D. Ellery
*/

// All-in-one .h file that has everything to include in each .cpp/.h file
#include "plugin_includes.h"

// Specify local code files here
#include "debug_logging.h"

#include "globals.h"
#include "config.h"

/*
    Set the most-important plugin settings.

    - Name
    - Description
    - Version
    - Author
    - License

    These are needed, otherwise this plugin will fail
*/

// Because variables can't be used on macros, we'll need to specify everything separately :T

WUPS_PLUGIN_NAME("PowerLEDColorU");
WUPS_PLUGIN_DESCRIPTION("Allows you to change the Wii U's LED Color, located on the Power Button, while the console is turned on");
WUPS_PLUGIN_VERSION("v1.0.0");
WUPS_PLUGIN_AUTHOR("Solace D. Ellery");
WUPS_PLUGIN_LICENSE("MIT");

// Set to use a config file
WUPS_USE_STORAGE("PowerLEDColorU");

// Used for debug logs
WUPS_USE_WUT_DEVOPTAB();

// --------

/*
void Overlay_Debug_Stats(wups_overlay_options_type_t screen, void * args)
{
    while (GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay)
    {
        int Text_Height = 5;

        WUPS_Overlay_OSScreenClear(screen);

        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 0), "PowerLEDColorU");
        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 1), "DEBUG OVERLAY: STATS");
        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 2), "----------------");

        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 4), "Is Plugin Enabled? " + GlobalVarsFuncs.booleanToAnswerIndicator(GlobalVarsFuncs.PluginConfigSettings_EnablePlugin));

        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 6), "--------");

        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 8), "Is LED On? " + GlobalVarsFuncs.booleanToAnswerIndicator(GlobalVarsFuncs.PluginConfigSettings_EnableLED));
        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 9), "Is LED Blinking? " + GlobalVarsFuncs.booleanToAnswerIndicator(GlobalVarsFuncs.PluginConfigSettings_IsBlinking));

        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 11), "--------");

        WUPS_Overlay_PrintTextOnScreen(screen, 0, (Text_Height * 13), "LED Color (Raw): " + std::to_string(GlobalVarsFuncs.PluginConfigSettings_ColorValue));
        
        WUPS_Overlay_FlipBuffers(screen);
    } 
}
*/

// --------

//bool debugOverlayOpened = false;

// Runs when the plugin first loads.
INITIALIZE_PLUGIN()
{
    // Reset the global variables
    Globals_Startup();

    // Initalize the logging system
    if (!WHBLogModuleInit())
    {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

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

    // Initalize the config
    Config::Init();

    // Notify that the plugin has officially started and is active, if on the Wii U menu
    if (GlobalVarsFuncs.isOnWiiUMenu())
    {
        GlobalVarsFuncs.displayNotification(GlobalVarsFuncs.PluginName + " is active and has started!");
    }

    /*if (GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay)
    {
        GlobalVarsFuncs.PluginDebugging_OverlayEnabled = true;
    }*/

    GlobalVarsFuncs.PluginConfig_PluginStarted = true;
}

// Runs when the plugin is about to close.
DEINITIALIZE_PLUGIN()
{
    // Plugin is now closing, so make this false
    GlobalVarsFuncs.PluginConfig_PluginStarted = false;

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
ON_APPLICATION_ENDS()
{

}

// Runs on every frame while the plugin is running.
int main()
{
    // Run anything at anytime while the plugin is officially active
    while(1)
    {
        // Runs stuff only if the plugin is officially started and beyond
        if(GlobalVarsFuncs.PluginConfigSettings_EnablePlugin && GlobalVarsFuncs.PluginConfig_PluginStarted)
        {
            if(GlobalVarsFuncs.PluginConfigSettings_IsBlinking)
            {
                
            }
            else
            {
                
            }
            /*if(GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay && !debugOverlayOpened)
            {
                WUPS_OpenOverlay(WUPS_OVERLAY_DRC_AND_TV, Overlay_Debug_Stats, NULL);
                debugOverlayOpened = true;
            }
            else if(((!GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay && debugOverlayOpened) || (!GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay)))
            {
                debugOverlayOpened = false;
            }*/
        }
    }
}
