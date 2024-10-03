#ifndef LEDCOLORU_CONFIG_H
#define LEDCOLORU_CONFIG_H

#include "plugin_includes.h"

class Config
{
public:
    static void Init();

    // wups config items
    static bool EnablePlugin;
    static bool EnableLED;

    static int32_t ColorValue;
    static bool IsBlinking;

    static bool EnableDebugOverlay;
};

#endif //LEDCOLORU_CONFIG_H
