#ifndef LEDCOLORU_CONFIG_H
#define LEDCOLORU_CONFIG_H

class Config {
public:
    static void Init();

    // wups config items
    static bool led_enabled;
    static int8_t color_value;
};

#endif //LEDCOLORU_CONFIG_H
