#include "Update.h"
#include "../config/DisplayContext.h"
#include <sys/types.h>

static uint8_t cpu = 0;
static uint8_t load = 0;
static uint16_t uram = 0;
static uint16_t tram = 0;

void updateHeader(DisplayType& display, uint8_t temp, uint8_t load, uint8_t rpm, uint16_t uram, uint16_t tram){
    display.setPartialWindow(0, 0, 416, 30);


    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);

        display.setCursor(CPU_LABEL_X, HEADER_Y);
        display.print("CPU:");

        char cpuBuf[16];
        snprintf(cpuBuf, sizeof(cpuBuf), "%u C", temp);

        display.setCursor(CPU_VALUE_X, HEADER_Y);
        display.print(cpuBuf);

        display.setCursor(LOAD_LABEL_X, HEADER_Y);
        display.print("LOAD:");

        char loadBuf[16];
        snprintf(loadBuf, sizeof(loadBuf), "%u%%", load);

        display.setCursor(LOAD_VALUE_X, HEADER_Y);
        display.print(loadBuf);

        display.setCursor(225, HEADER_Y);
        display.print("12V:");

        // Y
        display.fillRect(100, 4, 3, 25, GxEPD_BLACK);
        display.fillRect(216, 4, 3, 25, GxEPD_BLACK);


        // H
        display.fillRect(1, 25, 415, 4, GxEPD_BLACK);
        
        

    } while (display.nextPage());

    display.setPartialWindow(0, 220, 416, 20);

    display.firstPage();
    do
    { 
        display.setCursor(RAM_LABEL_X, FOOTER_Y);
        display.print("RAM:");

        char ramBuf[32];
        snprintf(ramBuf,
                 sizeof(ramBuf),
                 "%.1f / %.1f GB",
                 uram / 10.0f,
                 tram / 10.0f);

        display.setCursor(RAM_VALUE_X, FOOTER_Y);
        display.print(ramBuf);  

        display.fillRect(1, 220, 415, 4, GxEPD_BLACK); // H
        display.fillRect(230, 220, 3, 25, GxEPD_BLACK); // Y

        display.setCursor(FAN_LABEL_X, FOOTER_Y);
        display.print("FAN:");

        char fanBuf[16];
        snprintf(fanBuf,
                sizeof(fanBuf),
                "%u%%",
                rpm);
        
        display.setCursor(FAN_VALUE_X, FOOTER_Y);
        display.print(fanBuf);

        display.fillRect(350, 220, 3, 25, GxEPD_BLACK);
        
    } while(display.nextPage());
}