#ifndef LEDCOLORU_CONFIG_H
#define LEDCOLORU_CONFIG_H

#include "plugin_includes.h"

class Config
{
public:
    static void Init();

    // wups config items
    static bool enabled;
    static bool led_enabled;

    static uint32_t color_value;
    static bool is_blinking;

    static bool debug_overlay_enabled;
};

#endif //LEDCOLORU_CONFIG_H
