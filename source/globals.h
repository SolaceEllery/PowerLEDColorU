#ifndef LEDCOLORU_GLOBALS_H
#define LEDCOLORU_GLOBALS_H

#include <cstdint>
#include <string_view>
#include <string>
#include <vector>

#include <wups.h>
#include <wut.h>
#include <sysapp/title.h>

#include <coreinit/bsp.h>
#include <coreinit/ios.h>

struct Global
{
    // Plugin Globals: Name
    std::string PluginName;
    
    // Plugin Globals: Description
    std::string PluginDescription;

    // Plugin Globals: Version Number
    int PluginVersion_Major;
    int PluginVersion_Moderate;
    int PluginVersion_Minor;

    std::string PluginVersion;
    std::string PluginVersionFull;

    // Plugin Globals: Author
    std::string PluginAuthor_First;
    std::string PluginAuthor_Middle;
    std::string PluginAuthor_Last;

    std::string PluginAuthor;

    // --------

    // Plugin Globals: Strings

    std::vector<std::string> PluginConfigStrings_MenuSections;
    std::vector<std::string> PluginConfigStrings_MenuOptions;

    // --------

    // Plugin Globals: Option Values

    uint32_t PluginConfig_ColorValue;
    bool PluginConfig_EnableLED;
    bool PluginConfig_IsBlinking;
    bool PluginConfig_EnablePlugin;

    // --------
    
    Global()
    {
        Reset();
    }

    void Reset()
    {
        // Plugin Globals: Name
        PluginName = "PowerLEDColorU";
        
        // Plugin Globals: Description
        PluginDescription = "Allows you to change the Wii U's LED Color, located on the Power Button, while the console is turned on";

        // Plugin Globals: Version Number
        PluginVersion_Major = 1;
        PluginVersion_Moderate = 0;
        PluginVersion_Minor = 0;

        // Plugin Globals: Author
        PluginAuthor_First = "Solace";
        PluginAuthor_Middle = "D.";
        PluginAuthor_Last = "Ellery";

        // Plugin Globals: Strings
        std::vector<std::string> PluginConfigStrings_MenuSections = {"Light Settings"};
        std::vector<std::string> PluginConfigStrings_MenuOptions = {"Enable LED Light", "Color Choice", "Enable Blinking", "Enable Plugin"};
        std::string PluginConfigStrings_MenuSeparator = "----------------";

        // Plugin Globals: Option Values
        PluginConfig_ColorValue = 0x20;
        PluginConfig_EnableLED = true;
        PluginConfig_IsBlinking = false;
        PluginConfig_EnablePlugin = true;
    }

    // True if on the Wii U menu, else this is false
    bool isOnWiiUMenu()
    {
        uint64_t current_title_id = OSGetTitleID();
        uint64_t wiiu_menu_tid = _SYSGetSystemApplicationTitleId(SYSTEM_APP_ID_WII_U_MENU);

        return (current_title_id == wiiu_menu_tid);
    }

    // This sets the Power LED state, by writing a value over to the BSP in the console.
    int32_t setPowerLEDState(uint8_t stateValue)
    {
        // This one function should do everything for us on this program
        return bspWrite("SMC", 0, "NotificationLED", 1, &stateValue);
    }
};

extern Global GlobalVarsFuncs;

void Globals_Startup()
{
    // Reset to the default values
    GlobalVarsFuncs.Reset();

    // Set all previous unset variables
    GlobalVarsFuncs.PluginVersion = std::to_string(GlobalVarsFuncs.PluginVersion_Major) + std::to_string(GlobalVarsFuncs.PluginVersion_Moderate) + std::to_string(GlobalVarsFuncs.PluginVersion_Minor);
    GlobalVarsFuncs.PluginVersionFull = "v" + std::to_string(GlobalVarsFuncs.PluginVersion_Major) + std::to_string(GlobalVarsFuncs.PluginVersion_Moderate) + std::to_string(GlobalVarsFuncs.PluginVersion_Minor);

    GlobalVarsFuncs.PluginAuthor = GlobalVarsFuncs.PluginAuthor_First + " " + GlobalVarsFuncs.PluginAuthor_Middle + " " + GlobalVarsFuncs.PluginAuthor_Last;
}

#endif //LEDCOLORU_CONFIG_H
