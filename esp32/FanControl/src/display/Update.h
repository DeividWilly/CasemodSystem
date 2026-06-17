#pragma once

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold12pt7b.h>

using DisplayType = GxEPD2_BW<GxEPD2_370_GDEY037T03,
                               GxEPD2_370_GDEY037T03::HEIGHT>;

void updateHeader(DisplayType& display, uint8_t temp, uint8_t load, uint8_t rpm, uint16_t uram, uint16_t tram);