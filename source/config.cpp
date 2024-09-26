#include "config.h"

#include "plugin_includes.h"

#include "debug_logging.h"
#include "globals.h"

// Function to change the plugin status. Used when setting an option in the plugin menu
static void PluginConfigFunctions_TogglePlugin(ConfigItemBoolean* item, bool new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Enable Plugin' config value to '%d'...", new_value);

    // Change the variable
    GlobalVarsFuncs.PluginConfigSettings_EnablePlugin = new_value;

    // Revert LED & debug overlay to the default settings
    if (!GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfigDefaults_ColorValue);
        GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay = false;
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>("enabled", GlobalVarsFuncs.PluginConfigSettings_EnablePlugin) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the \"enabled\" config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_EnablePlugin);
    }
}

// Function to change the LED status
static void PluginConfigFunctions_ToggleLED(ConfigItemBoolean* item, bool new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Enable LED' config value to '%d'...", new_value);

    // Change the variable
    if (GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        GlobalVarsFuncs.PluginConfigSettings_EnableLED = new_value;

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

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>("led_enabled", GlobalVarsFuncs.PluginConfigSettings_EnableLED) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'led_enabled' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_EnableLED);
    }
}


// Function to change the LED color.
static void PluginConfigFunctions_ChangeColor(ConfigItemIntegerRange* item, int32_t new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Color Choice' config value to '%d'...", new_value);

    // Change the variable
    GlobalVarsFuncs.PluginConfigSettings_ColorValue = (uint8_t)new_value;

    // Only change the color when the plugin is on, and the LED is enabled
    if(GlobalVarsFuncs.PluginConfigSettings_EnablePlugin && GlobalVarsFuncs.PluginConfigSettings_EnableLED)
    {
        // Switch the color to the new one
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfigSettings_ColorValue);
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<uint32_t>("color_value", GlobalVarsFuncs.PluginConfigSettings_ColorValue) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'color_value' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_ColorValue);
    }
}

// Function to change the LED blinking status.
static void PluginConfigFunctions_ToggleBlinking(ConfigItemBoolean* item, bool new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Enable Blinking' config value to '%d'...", new_value);

    // Change the variable
    if (GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        GlobalVarsFuncs.PluginConfigSettings_IsBlinking = new_value;

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

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>("is_blinking", GlobalVarsFuncs.PluginConfigSettings_IsBlinking) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the \"is_blinking\" config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_IsBlinking);
    }
}


// Function to change the debug overlay status
static void PluginConfigFunctions_ToggleDebugOverlay(ConfigItemBoolean* item, bool new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Enable Debug Overlay' config value to '%d'...", new_value);

    // Change the variable
    if (GlobalVarsFuncs.PluginConfigSettings_EnablePlugin)
    {
        GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay = new_value;
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>("debug_overlay_enabled", GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'debug_overlay_enabled' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay);
    }
}


// The function that sets up the plugin's menus.
static WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    // Create the root config category
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    // Add the options over to the root of the plugin's menu, and sections as empty text separators

    // -- Enable Plugin --
    root.add(WUPSConfigItemBoolean::Create("enabled", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[3], true, GlobalVarsFuncs.PluginConfigSettings_EnablePlugin, &PluginConfigFunctions_TogglePlugin));

    // --------

    // [-- LED SETTINGS --]
    //root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections[0]));
    //root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- Enable LED Light --
    root.add(WUPSConfigItemBoolean::Create("led_enabled", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[0], true, GlobalVarsFuncs.PluginConfigSettings_EnableLED, &PluginConfigFunctions_ToggleLED));

    // -- Enable LED Blinking --
    root.add(WUPSConfigItemBoolean::Create("is_blinking", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[2], false, GlobalVarsFuncs.PluginConfigSettings_IsBlinking, &PluginConfigFunctions_ToggleBlinking));

    // -- LED Color --
    root.add(WUPSConfigItemIntegerRange::Create("color_value", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[1], GlobalVarsFuncs.PluginConfigSettings_ColorValue, GlobalVarsFuncs.PluginConfigSettings_ColorValue, 0x01, 0xFF, &PluginConfigFunctions_ChangeColor));

    // [-- DEBUG SETTINGS --]
    /*root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections[1]));
    root.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- Enable Debug Overlay --
    root.add(WUPSConfigItemBoolean::Create("debug_overlay_enabled", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[4], false, GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay, &PluginConfigFunctions_ToggleDebugOverlay));*/

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
    WUPSConfigAPIOptionsV1 configOptions = {.name = GlobalVarsFuncs.PluginName.c_str()};

    // Make sure it didn't fail to initalize
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to initialize WUPS Config API");
        return;
    }

    // Create the storage error variable
    WUPSStorageError storageRes;

    // Try to get values from storage, and if anything is missing, add the values to the storage config



    // --------

    // -- Enable Plugin --

    // Get the value from the config file
    storageRes = WUPSStorageAPI::Get<bool>("enabled", GlobalVarsFuncs.PluginConfigSettings_EnablePlugin);

    // If there is nothing, create the config value with the default settings
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        // Let the user know the plugin is creating a new value
        GlobalVarsFuncs.displayNotification("PowerLEDColorU: No 'Enable Plugin' value set, setting default setting (On)...");

        // Store the default value to the main variable
        GlobalVarsFuncs.PluginConfigSettings_EnablePlugin = GlobalVarsFuncs.PluginConfigDefaults_EnablePlugin;

        // Store the default value to the plugin config, else error out
        if (WUPSStorageAPI::Store<bool>("enabled", GlobalVarsFuncs.PluginConfigDefaults_EnablePlugin) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'enabled' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_EnablePlugin);
        }
    }

    // If nothing still works, we couldn't read where the config should be at
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to read the 'enabled' config value (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // -- Enable LED --

    // Get the value from the config file
    storageRes = WUPSStorageAPI::Get<bool>("led_enabled", GlobalVarsFuncs.PluginConfigSettings_EnableLED);

    // If there is nothing, create the config value with the default settings
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        // Let the user know the plugin is creating a new value
        GlobalVarsFuncs.displayNotification("PowerLEDColorU: No 'LED Enabled' value set, setting default setting (On)...");

        // Store the default value to the main variable
        GlobalVarsFuncs.PluginConfigSettings_EnableLED = GlobalVarsFuncs.PluginConfigDefaults_EnableLED;

        // Store the default value to the plugin config, else error out
        if (WUPSStorageAPI::Store<bool>("led_enabled", GlobalVarsFuncs.PluginConfigDefaults_EnableLED) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'led_enabled' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_EnableLED);
        }
    }

    // If nothing still works, we couldn't read where the config should be at
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to read the 'led_enabled' config value (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // ----

    // -- Color Value --

    // Get the value from the config file
    storageRes = WUPSStorageAPI::Get<uint32_t>("color_value", GlobalVarsFuncs.PluginConfigSettings_ColorValue);

    // If there is nothing, create the config value with the default settings
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        // Let the user know the plugin is creating a new value
        GlobalVarsFuncs.displayNotification("PowerLEDColorU: No 'Color' value set, setting default setting (Blue)...");

        // Store the default value to the main variable
        GlobalVarsFuncs.PluginConfigSettings_ColorValue = GlobalVarsFuncs.PluginConfigDefaults_ColorValue;

        // Store the default value to the plugin config, else error out
        if (WUPSStorageAPI::Store<uint32_t>("color_value", GlobalVarsFuncs.PluginConfigDefaults_ColorValue) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'color_value' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_ColorValue);
        }
    }

    // If nothing still works, we couldn't read where the config should be at
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to read the 'color_value' config value (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // -- Enable Blinking --

    // Get the value from the config file
    storageRes = WUPSStorageAPI::Get<bool>("is_blinking", GlobalVarsFuncs.PluginConfigSettings_IsBlinking);

    // If there is nothing, create the config value with the default settings
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        // Let the user know the plugin is creating a new value
        GlobalVarsFuncs.displayNotification("PowerLEDColorU: No 'Enable Blinking' value set, setting default setting (Off)...");

        // Store the default value to the main variable
        GlobalVarsFuncs.PluginConfigSettings_IsBlinking = GlobalVarsFuncs.PluginConfigDefaults_IsBlinking;

        // Store the default value to the plugin config, else error out
        if (WUPSStorageAPI::Store<bool>("is_blinking", GlobalVarsFuncs.PluginConfigDefaults_IsBlinking) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'is_blinking' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_IsBlinking);
        }
    }

    // If nothing still works, we couldn't read where the config should be at
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to read the 'is_blinking' config value (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // --------

    // -- Enable Debug Overlay --

    // Get the value from the config file
    storageRes = WUPSStorageAPI::Get<bool>("debug_overlay_enabled", GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay);

    // If there is nothing, create the config value with the default settings
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        // Log that the default setting is being set
        DEBUG_FUNCTION_LINE("PowerLEDColorU: No 'Enable Debug Overlay value set, setting default setting (Off)...");

        // Store the default value to the main variable
        GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay = GlobalVarsFuncs.PluginConfigDefaults_EnableDebugOverlay;

        // Store the default value to the plugin config, else error out
        if (WUPSStorageAPI::Store<bool>("debug_overlay_enabled", GlobalVarsFuncs.PluginConfigDefaults_EnableDebugOverlay) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'debug_overlay_enabled' config value (Value that was specified - '%d')", GlobalVarsFuncs.PluginConfigSettings_EnableDebugOverlay);
        }
    }

    // If nothing still works, we couldn't read where the config should be at
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to read the 'debug_overlay_enabled' config value (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // --------



    // Save all the values over to the config file
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the config file. Something must have gone horribly wrong.");
    }
}
