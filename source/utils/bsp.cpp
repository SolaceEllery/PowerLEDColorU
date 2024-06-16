#include <stdint.h>
#include <wut.h>
#include <coreinit/bsp.h>

#include "bsp.h"

int SetNotificationLED(uint8_t mask)
{
    return bspWrite("SMC", 0, "NotificationLED", 1, &mask);
}
