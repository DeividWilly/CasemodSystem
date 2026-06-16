#include "DisplayDriver.h"
#include "../config/Pins.h"
#include "assets/Inconsolata_Bold12pt7b.h"
#include <GxEPD2_BW.h>
#include <SPI.h>

GxEPD2_BW<GxEPD2_370_GDEY037T03,
           GxEPD2_370_GDEY037T03::HEIGHT>
           display(
            GxEPD2_370_GDEY037T03(
                Pins::EPD_CS,
                Pins::EPD_DC,
                Pins::EPD_RES,
                Pins::EPD_BUSY
            )
);

void displayInit(){
    SPI.begin(Pins::EPD_SCK, -1, Pins::EPD_MOSI, Pins::EPD_CS);
    
    pinMode(Pins::EPD_BUSY, INPUT);

    display.init(115200);

    display.setRotation(1);
    display.setFont(&Inconsolata_Bold12pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setFullWindow();
}