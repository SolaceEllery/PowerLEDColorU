#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <coreinit/bsp.h>

int SetNotificationLED(uint8_t mask);

enum {
    // No LED: LED Off
    NOTIF_LED_OFF               = 0x00,
    // Yellow LED: Blinking
    NOTIF_LED_YELLOW_BLINKING   = 0x01,
    // Yellow LED: Normal
    NOTIF_LED_YELLOW            = 0x02,
    // Red LED: Blinking
    NOTIF_LED_RED_BLINKING      = 0x04,
    // Orange LED: Blinking
    NOTIF_LED_ORANGE_BLINKING   = 0x05,
    // Red LED: Normal
    NOTIF_LED_RED               = 0x08,
    // Orange LED: Normal (10)
    NOTIF_LED_ORANGE            = 0x0A,
    // Blue LED: Blinking (16)
    NOTIF_LED_BLUE_BLINKING     = 0x10,
    // Gray LED: Blinking (17)
    NOTIF_LED_GRAY_BLINKING     = 0x11,
    // Purple LED: Blinking (20)
    NOTIF_LED_PURPLE_BLINKING   = 0x14,
    // Blue LED: Normal (32)
    NOTIF_LED_BLUE              = 0x20,
    // Gray LED: Normal (34)
    NOTIF_LED_GRAY              = 0x22,
    // Purple LED: Normal (40)
    NOTIF_LED_PURPLE            = 0x28,
};

#endif // LED_H
