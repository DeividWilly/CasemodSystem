#pragma once

#include <stdint.h>

struct __attribute__((packed)) Packet
{
    uint8_t temp;
    uint8_t load;
    uint8_t rpm;
    uint16_t uram;
    uint16_t tram;
};