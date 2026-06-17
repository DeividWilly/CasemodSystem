#pragma once
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold12pt7b.h>

using DisplayType = GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT>;
void drawStaticLayout(DisplayType& display);
