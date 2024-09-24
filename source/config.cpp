#include "config.h"
#include "globals.h"

#include "utils/logger.h"
#include "utils/bsp.h"

#include <wups.h>
#include <wups/storage.h>
#include <wups/config_api.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>

#include <coreinit/title.h>
#include <sysapp/title.h>

#include <string>
#include <cstdint>
#include <vector>

// Function to change the LED status. Used when setting an option in the plugin menu
static void PluginConfig_ToggleLED(ConfigItemBoolean* item, bool new_value)
{
    DEBUG_FUNCTION_LINE("led_enabled changed to: %d", new_value);
    GlobalVarsFuncs.PluginConfig_EnableLED = new_value;

    if (!GlobalVarsFuncs.PluginConfig_EnableLED)
    {
        GlobalVarsFuncs.setPowerLEDState(0x00);
    }
    else
    {
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfig_ColorValue);
    }

    if (WUPSStorageAPI::Store<bool>("led_enabled", GlobalVarsFuncs.PluginConfig_EnableLED) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save \"led_enabled\" value (%d)", GlobalVarsFuncs.PluginConfig_EnableLED);
    }
}

// Function to change the LED color.
static void PluginConfig_ChangeColor(ConfigItemIntegerRange* item, int32_t new_value) {
    if (GlobalVarsFuncs.PluginConfig_EnableLED)
    {
        DEBUG_FUNCTION_LINE("color_value changed to: %d", new_value);
        GlobalVarsFuncs.PluginConfig_ColorValue = (uint8_t)new_value;
        GlobalVarsFuncs.setPowerLEDState(GlobalVarsFuncs.PluginConfig_ColorValue);
        if (WUPSStorageAPI::Store<uint32_t>("color_value", GlobalVarsFuncs.PluginConfig_ColorValue) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("Failed to save \"color_value\" value (%d)", GlobalVarsFuncs.PluginConfig_ColorValue);
        }
    }
}

// Function to change the LED blinking status.
static void PluginConfig_ToggleBlinking(ConfigItemBoolean* item, bool new_value)
{
    DEBUG_FUNCTION_LINE("is_blinking changed to: %d", new_value);
    GlobalVarsFuncs.PluginConfig_IsBlinking = new_value;

    if (!GlobalVarsFuncs.PluginConfig_IsBlinking)
    {
        
    }
    else
    {
        
    }

    if (WUPSStorageAPI::Store<bool>("is_blinking", GlobalVarsFuncs.PluginConfig_IsBlinking) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save \"is_blinking\" value (%d)", GlobalVarsFuncs.PluginConfig_IsBlinking);
    }
}

// Function to change the plugin status.
static void PluginConfig_TogglePlugin(ConfigItemBoolean* item, bool new_value)
{
    DEBUG_FUNCTION_LINE("enabled changed to: %d", new_value);
    GlobalVarsFuncs.PluginConfig_EnablePlugin = new_value;

    if (WUPSStorageAPI::Store<bool>("enabled", GlobalVarsFuncs.PluginConfig_EnablePlugin) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save \"enabled\" value (%d)", GlobalVarsFuncs.PluginConfig_EnablePlugin);
    }
}

// The function that sets up the plugin's menus.
static WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    // Create the root config category
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    // Add the options over to the root of the plugin's menu, and sections as empty text separators

    // -- Enable Plugin --
    root.add(WUPSConfigItemBoolean::Create("enabled", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[3], true, GlobalVarsFuncs.PluginConfig_EnablePlugin, &PluginConfig_TogglePlugin));

    // [-- LIGHT SETTINGS --]
    root.add(WUPSConfigItemText::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));
    root.add(WUPSConfigItemText::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections));
    root.add(WUPSConfigItemText::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- Enable LED Light --
    root.add(WUPSConfigItemBoolean::Create("led_enabled", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[0], true, GlobalVarsFuncs.PluginConfig_EnableLED, &PluginConfig_ToggleLED));

    // -- Color Choice --
    root.add(WUPSConfigItemIntegerRange::Create("color_value", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[1], GlobalVarsFuncs.PluginConfig_ColorValue, GlobalVarsFuncs.PluginConfig_ColorValue, 0x01, 0xFF, &PluginConfig_ChangeColor));

    // -- Enable Blinking --
    root.add(WUPSConfigItemBoolean::Create("is_blinking", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[2], false, GlobalVarsFuncs.PluginConfig_IsBlinking, &PluginConfig_ToggleBlinking));

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

    WUPSStorageError storageRes;

    // Try to get values from storage, and if anything is missing, add the values to the storage config

    // LED Enabled
    storageRes = WUPSStorageAPI::Get<bool>("led_enabled", GlobalVarsFuncs.PluginConfig_EnableLED);
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        ShowNotification(GlobalVarsFuncs.PluginName + ": No 'LED Enabled' value set, setting default setting (On)...");
        DEBUG_FUNCTION_LINE("Detected no LED Enable value, attempting to migrate/create...");
        // --LED Toggle--
        if (WUPSStorageAPI::Store<bool>("led_enabled", GlobalVarsFuncs.PluginConfig_EnableLED) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("Failed to store 'led enable' bool");
        }
    }
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to get bool %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // Color Value
    storageRes = WUPSStorageAPI::Get<uint32_t>("color_value", GlobalVarsFuncs.PluginConfig_ColorValue);
    if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        ShowNotification(GlobalVarsFuncs.PluginName + ": No 'Color' value set, setting default setting (Blue)...");
        DEBUG_FUNCTION_LINE("Detected no LED color setting, attempting to migrate/create...");
        // --LED Toggle--
        if (WUPSStorageAPI::Store<uint32_t>("color_value", GlobalVarsFuncs.PluginConfig_ColorValue) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("Failed to store 'color_value' bool");
        }
    }
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to get uint32_t %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }


    // Save storage
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save storage");
    }
}
