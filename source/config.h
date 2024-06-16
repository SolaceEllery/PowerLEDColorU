#ifndef LEDCOLORU_CONFIG_H
#define LEDCOLORU_CONFIG_H

#include <cstdint>
#include <string_view>

class Config
{
public:
    static void Init();

    // wups config items
    static uint8_t color_value;
    static bool led_enabled;
};

extern void ShowNotification(std::string_view notification);

#endif //LEDCOLORU_CONFIG_H
