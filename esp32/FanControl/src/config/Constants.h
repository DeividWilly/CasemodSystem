#pragma once

#include <stdint.h>

namespace Constants
{
    constexpr uint32_t SERIAL_BAUD = 115200;
    constexpr uint8_t PWM_CHANNELS[] = {0, 1, 2, 3};
    constexpr uint8_t RESOLUTION = 8;
    constexpr uint16_t FREQUENCY = 25000;

    constexpr uint8_t DISPLAY_QUEUE_SIZE = 1;
    constexpr uint8_t PWM_QUEUE_SIZE = 1;
}