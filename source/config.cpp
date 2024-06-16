#include "config.h"

#include "utils/logger.h"
#include "utils/sysconfig.h"
#include "utils/bsp.h"

#include <wups.h>
#include <wups/storage.h>
#include <wups/config_api.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>

#include <string>
#include <cstdint>

int8_t Config::color_value = 0x20;
bool Config::led_enabled = true;

std::string plugin_name = "PowerLEDColorU";
std::string light_settings_string = "Light Settings";
std::string enable_led_light = "Enable LED Light";
std::string color_to_use = "Color to use";

static void led_enabled_changed(ConfigItemBoolean* item, bool new_value) {
    DEBUG_FUNCTION_LINE("led_enabled changed to: %d", new_value);
    Config::led_enabled = new_value;

    if (!Config::led_enabled)
    {
        SetNotificationLED(0x00);
    }
    else
    {
        SetNotificationLED(Config::color_value);
    }

    if (WUPSStorageAPI::Store<bool>("led_enabled", Config::led_enabled) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to save \"led_enabled\" value (%d)", Config::led_enabled);
    }
}

static void color_value_changed(ConfigItemIntegerRange* item, int32_t new_value) {
    if (Config::led_enabled)
    {
        DEBUG_FUNCTION_LINE("color_value changed to: %d", new_value);
        Config::color_value = (int8_t)new_value;
        SetNotificationLED(Config::color_value);
        if (WUPSStorageAPI::Store<int8_t>("color_value", Config::color_value) != WUPS_STORAGE_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE("Failed to save \"color_value\" value (%d)", Config::color_value);
        }
    }
}

static WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    // create root config category
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);
    auto light_settings = WUPSConfigCategory::Create(light_settings_string);

    // LED Enabled
    light_settings.add(WUPSConfigItemBoolean::Create("led_enabled", enable_led_light, true, Config::led_enabled, &led_enabled_changed));

    // Color value
    light_settings.add(WUPSConfigItemIntegerRange::Create("color_value", color_to_use, (int32_t)Config::color_value, (int32_t)Config::color_value, (int32_t)0x01, (int32_t)0xFF, &color_value_changed));
}

static void ConfigMenuClosedCallback() {
    // Save all changes
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to save storage");
    }
}

void Config::Init() {
    // Init the config api
    WUPSConfigAPIOptionsV1 configOptions = { .name = "PowerLEDColorU" };
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to initialize WUPS Config API");
        return;
    }

    WUPSStorageError storageRes;

    // Try to get values from storage, and if anything is missing, add the values to the storage config

    // LED Enabled
    if ((storageRes == WUPSStorageAPI::Get<bool>("led_enabled", Config::led_enabled)) == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        DEBUG_FUNCTION_LINE("Detected no LED Enable value, attempting to migrate/create...");
        // --LED Toggle--
        if (WUPSStorageAPI::Store<bool>("led_enabled", Config::led_enabled) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("Failed to store 'led enable' bool");
        }
    }
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to get bool %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // Color Value
    if ((storageRes == WUPSStorageAPI::Get<int8_t>("color_value", Config::color_value)) == WUPS_STORAGE_ERROR_NOT_FOUND)
    {
        DEBUG_FUNCTION_LINE("Detected no LED color setting, attempting to migrate/create...");
        // --LED Toggle--
        if (WUPSStorageAPI::Store<int8_t>("color_value", Config::color_value) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("Failed to store 'color_value' bool");
        }
    }
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to get int8_t %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }


    // Save storage
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save storage");
    }
}
