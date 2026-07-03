#pragma once

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold12pt7b.h>

using DisplayType = GxEPD2_BW<GxEPD2_370_GDEY037T03,
                               GxEPD2_370_GDEY037T03::HEIGHT>;

void updateDisplay(DisplayType& display, 
                uint8_t temp, 
                uint8_t load, 
                uint8_t rpm, 
                uint16_t uram, 
                uint16_t tram, 
                float fontTemperature,
                float voltage12,
                float current12,
                float power12,
                float voltage19,
                float current19,
                float power19
            );
