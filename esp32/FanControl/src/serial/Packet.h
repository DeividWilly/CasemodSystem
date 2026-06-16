#pragma once

#include <stdint.h>

struct Packet
{
    uint8_t temp;
    uint8_t load;
    uint8_t rpm;
    uint16_t uram;
    uint16_t tram;
};