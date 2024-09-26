#ifndef LEDCOLORU_GLOBALS_H
#define LEDCOLORU_GLOBALS_H

#include "plugin_includes.h"

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
    std::string PluginConfigStrings_MenuSeparator;

    // --------

    // Plugin Globals: System Option Values
    bool PluginConfig_PluginStarted;

    // Plugin Globals: Default Option Values
    bool PluginConfigDefaults_EnablePlugin;
    bool PluginConfigDefaults_EnableLED;

    uint32_t PluginConfigDefaults_ColorValue;
    bool PluginConfigDefaults_IsBlinking;

    bool PluginConfigDefaults_EnableDebugOverlay;


    // Plugin Globals: Option Values
    bool PluginConfigSettings_EnablePlugin;
    bool PluginConfigSettings_EnableLED;

    uint32_t PluginConfigSettings_ColorValue;
    bool PluginConfigSettings_IsBlinking;

    bool PluginConfigSettings_EnableDebugOverlay;
    

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
        std::vector<std::string> PluginConfigStrings_MenuSections = {"LED Settings", "Debug Settings"};
        std::vector<std::string> PluginConfigStrings_MenuOptions = {"Enable LED Light", "LED Color", "Enable LED Blinking", "Enable Plugin", "Enable Debug Overlay"};
        std::string PluginConfigStrings_MenuSeparator = "----------------";

        // --------

        // Plugin Globals: System Option Values
        PluginConfig_PluginStarted = false;


        // Plugin Globals: Default Option Values (Never changed)
        PluginConfigDefaults_EnablePlugin = true;
        PluginConfigDefaults_EnableLED = true;

        PluginConfigDefaults_ColorValue = 0x20;
        PluginConfigDefaults_IsBlinking = false;

        PluginConfigDefaults_EnableDebugOverlay = false;


        // Plugin Globals: Option Values
        PluginConfigSettings_EnablePlugin = true;
        PluginConfigSettings_EnableLED = true;

        PluginConfigSettings_ColorValue = 0x20;
        PluginConfigSettings_IsBlinking = false;

        PluginConfigSettings_EnableDebugOverlay = false;

        // --------
    }

    // True if on the Wii U menu, else this is false
    bool isOnWiiUMenu()
    {
        uint64_t current_title_id = OSGetTitleID();
        uint64_t wiiu_menu_tid = _SYSGetSystemApplicationTitleId(SYSTEM_APP_ID_WII_U_MENU);

        return (current_title_id == wiiu_menu_tid);
    }

    // Retrieves the Power LED state
    int32_t getPowerLEDState()
    {
        return bspRead("SMC", 1, "NotificationLED", 1, NULL);
    }

    // This sets the Power LED state, by writing a value over to the BSP in the console.
    int32_t setPowerLEDState(uint8_t stateValue)
    {
        // This one function should do everything for us on this program
        return bspWrite("SMC", 0, "NotificationLED", 1, &stateValue);
    }

    // Shows a notification on the Wii U's screens, on the top-left corner
    void displayNotification(std::string_view notification)
    {
        // 
        auto err1 = NotificationModule_SetDefaultValue(NOTIFICATION_MODULE_NOTIFICATION_TYPE_INFO, NOTIFICATION_MODULE_DEFAULT_OPTION_KEEP_UNTIL_SHOWN, true);
        auto err2 = NotificationModule_SetDefaultValue(NOTIFICATION_MODULE_NOTIFICATION_TYPE_INFO, NOTIFICATION_MODULE_DEFAULT_OPTION_DURATION_BEFORE_FADE_OUT, 15.0f);

        if (err1 != NOTIFICATION_MODULE_RESULT_SUCCESS || err2 != NOTIFICATION_MODULE_RESULT_SUCCESS)
        {
            return;
        }

        NotificationModule_AddInfoNotification(notification.data());
    }

    std::string booleanToSwitchIndicator(bool value)
    {
        if(value)
        {
            return "On";
        }
        else
        {
            return "Off";
        }
    }

    std::string booleanToAnswerIndicator(bool value)
    {
        if(value)
        {
            return "Yes";
        }
        else
        {
            return "No";
        }
    }
};

extern Global GlobalVarsFuncs;

extern void Globals_Startup();

#endif //LEDCOLORU_CONFIG_H
