#include "Update.h"
#include "../config/DisplayContext.h"
#include "DisplayDriver.h"
#include <cstdio>
#include <sys/types.h>

static uint8_t cpu = 0;
static uint8_t load = 0;
static uint16_t uram = 0;
static uint16_t tram = 0;

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
                float power19,
                bool rotate){
    display.setPartialWindow(0, 0, 416, 30);


    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        display.fillRect(1, 25, 415, 4, GxEPD_BLACK); // HORIZONTAL LINE

        display.fillRect(100, 4, 3, 25, GxEPD_BLACK); // VERTICAL DIVISOR 1
        display.fillRect(216, 4, 3, 25, GxEPD_BLACK); // VERTICAL DIVISOR 2

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

        if(rotate == true){

            display.setCursor(222, HEADER_Y);
            display.print("12V:");

            char volt12Buf[32];
            snprintf(volt12Buf,
                    sizeof(volt12Buf),
                    "%.1fv %.1fW",
                    voltage12,
                    power12);

            display.setCursor(VOLTAGE_12_VALUE, HEADER_Y);
            display.print(volt12Buf);
        } else {

            display.setCursor(222, HEADER_Y);
            display.print("19V:");

            char volt19Buf[32];
            snprintf(volt19Buf,
                    sizeof(volt19Buf),
                    "%.1fv %.1fW",
                    voltage19,
                    power19);

            display.setCursor(VOLTAGE_12_VALUE, HEADER_Y);
            display.print(volt19Buf);
        }

    } while (display.nextPage());

    display.setPartialWindow(0, 220, 416, 20);

    display.firstPage();
    do
    { 
        display.fillScreen(GxEPD_WHITE);

        display.fillRect(1, 220, 415, 4, GxEPD_BLACK); // HORIZONTAL LINE
        display.fillRect(230, 220, 3, 25, GxEPD_BLACK); // VERTICAL DIVISOR

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

        char tempBuf[32];
        snprintf(tempBuf, 
            sizeof(tempBuf), 
            "%.0f C", 
            fontTemperature);
            
        display.setCursor(FONT_TEMP_VALUE_X, FOOTER_Y);
        display.print(tempBuf);
        
    } while(display.nextPage());
}