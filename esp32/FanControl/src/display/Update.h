#pragma once

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold12pt7b.h>

using DisplayType = GxEPD2_BW<GxEPD2_370_GDEY037T03,
                               GxEPD2_370_GDEY037T03::HEIGHT>;

void updateCPU(DisplayType& display, uint8_t value);
void updateLOAD(DisplayType& display, uint8_t value);
void updateURAM(DisplayType& display, uint16_t uram, uint16_t tram);
void updateFAN(DisplayType& display, uint8_t value);