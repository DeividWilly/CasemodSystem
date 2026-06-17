#pragma once
#include <GxEPD2_BW.h>
#include <SPI.h>

extern GxEPD2_BW<GxEPD2_370_GDEY037T03, GxEPD2_370_GDEY037T03::HEIGHT> display;

void displayInit();