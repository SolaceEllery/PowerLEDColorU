#ifndef LEDCOLORU_CONFIG_H
#define LEDCOLORU_CONFIG_H

#include <cstdint>
#include <string_view>
#include <vector>

class Config
{
public:
    static void Init();

    // wups config items
    static uint32_t color_value;
    static bool led_enabled;
    static bool is_blinking;
};

extern void ShowNotification(std::string_view notification);

#endif //LEDCOLORU_CONFIG_H
