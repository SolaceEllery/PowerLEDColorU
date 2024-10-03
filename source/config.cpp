#include "config.h"

#include "plugin_includes.h"

#include "debug_logging.h"
#include "globals.h"

// Function to change the plugin status. Used when setting an option in the plugin menu
static void PluginConfigFunctions_EnablePlugin(ConfigItemBoolean* item, bool new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_EnablePlugin, new_value, "enabled");

    // Revert LED & debug overlay to the default settings
    if (!GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfigDefaults_ColorValue);
        GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay = false;
    }
    else
    {
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfigSettings_ColorValue);
    }
}

// Function to change the LED status
static void PluginConfigFunctions_EnableLED(ConfigItemBoolean* item, bool new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_EnableLED, new_value, "led_enabled");

    // Check for if the plugin is enabled
    if (GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        // Set the power LED state depending on if the LED should be on or not
        if (!GlobalVarsFuncs.PluginConfigSettings_EnableLED)
        {
            GlobalVarsFuncs.setPowerLEDState(0x00);
        }
        else
        {
            GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfigSettings_ColorValue);
        }
    }
}


// Function to change the LED color.
static void PluginConfigFunctions_ColorValue(ConfigItemIntegerRange* item, int32_t new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_ColorValue, new_value, "color_value");

    // Only change the color when the plugin is on, and the LED is enabled
    if((GlobalVarsFuncs.PluginConfigSettings_EnablePlugin) && (GlobalVarsFuncs.PluginConfigSettings_EnableLED))
    {
        // Switch the color to the new one
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfigSettings_ColorValue);
    }
}

// Function to change the LED blinking status.
static void PluginConfigFunctions_IsBlinking(ConfigItemBoolean* item, bool new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_IsBlinking, new_value, "is_blinking");

    // Change the variable
    if (GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        // Switch whatever color is specified to the matching blinking version of the same color
        if (!GlobalVarsFuncs.PluginConfigSettings_IsBlinking)
        {
            // WIP
        }
        else
        {
            // WIP
        }
    }
}


// Function to change the debug overlay status
static void PluginConfigFunctions_EnableDebugOverlay(ConfigItemBoolean* item, bool new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay, new_value, "debug_overlay_enabled");
}


// The function that sets up the plugin's menus.
static WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    // Create the root config category
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    // Add the options over to the root of the plugin's menu, and add sections as empty text separators

    // -- Enable Plugin --
    root.add(WUPSConfigItemBoolean::CreateEx("EnablePlugin", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[3], true, GlobalVarsFuncs.PluginConfigSettings_EnablePlugin, &PluginConfigFunctions_EnablePlugin, "On", "Off"));

    // --------

    // [-- LED SETTINGS --]
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections[0]));
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- Enable LED Light --
    root.add(WUPSConfigItemBoolean::CreateEx("EnableLED", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[0], true, GlobalVarsFuncs.PluginConfigSettings_EnableLED, &PluginConfigFunctions_EnableLED, "On", "Off"));

    // -- Enable LED Blinking --
    root.add(WUPSConfigItemBoolean::CreateEx("IsBlinking", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[2], false, GlobalVarsFuncs.PluginConfigSettings_IsBlinking, &PluginConfigFunctions_IsBlinking, "On", "Off"));

    // -- LED Color --
    root.add(WUPSConfigItemIntegerRange::Create("ColorValue", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[1], GlobalVarsFuncs.PluginConfigDefaults_ColorValue, GlobalVarsFuncs.PluginConfigSettings_ColorValue, 0x01, 0xFF, &PluginConfigFunctions_ColorValue));

    // [-- DEBUG SETTINGS --]
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections[1]));
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- Enable Debug Overlay --
    root.add(WUPSConfigItemBoolean::CreateEx("EnableDebugOverlay", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[4], false, GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay, &PluginConfigFunctions_EnableDebugOverlay, "On", "Off"));

    // Return with a result of success
    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

// This function runs when the plugin menu is closing.
static void ConfigMenuClosedCallback()
{
    // Save all changes
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save storage");
    }
}

// The main function that sets up all the config stuff.
void Config::Init()
{
    // Initalize the Config API
    WUPSConfigAPIOptionsV1 configOptions = {.name = "PowerLEDColorU"};

    // Make sure it didn't fail to initalize
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to initialize WUPS Config API");
        return;
    }

    // Try to get values from storage, and if anything is missing, add the values to the storage config

    // --------

    // -- Enable Plugin --
    GlobalVarsFuncs.readConfig_firstRead_Bool(GlobalVarsFuncs.PluginConfigSettings_EnablePlugin, GlobalVarsFuncs.PluginConfigDefaults_EnablePlugin, "Enable Plugin", "On", "enabled");

    // -- Enable LED --
    GlobalVarsFuncs.readConfig_firstRead_Bool(GlobalVarsFuncs.PluginConfigSettings_EnableLED, GlobalVarsFuncs.PluginConfigDefaults_EnableLED, "Enable LED Light", "On", "led_enabled");

    // ----

    // -- Color Value --
    GlobalVarsFuncs.readConfig_firstRead_Int32(GlobalVarsFuncs.PluginConfigSettings_ColorValue, GlobalVarsFuncs.PluginConfigDefaults_ColorValue, "LED Color", "Blue", "color_value");

    // -- Enable Blinking --
    GlobalVarsFuncs.readConfig_firstRead_Bool(GlobalVarsFuncs.PluginConfigSettings_IsBlinking, GlobalVarsFuncs.PluginConfigDefaults_IsBlinking, "Enable Blinking", "Off", "is_blinking");

    // --------

    // -- Enable Debug Overlay --
    GlobalVarsFuncs.readConfig_firstRead_Bool(GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay, GlobalVarsFuncs.PluginConfigDefaults_EnableDebugOverlay, "Enable Debug Overlay", "Off", "debug_overlay_enabled");

    // --------

    // Save all the values over to the config file
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the config file. Something must have gone horribly wrong.");
    }
}
