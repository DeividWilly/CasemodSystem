#include "Update.h"
#include "DisplayContext.h"
#include <set>

static const int X_LABEL = 10;
static const int X_VALUE = 90;

static const int LINE_H = 42;
static const int BASELINE_OFFSET = 30;

static const int Y_CPU  = 40;
static const int Y_LOAD = 82;
static const int Y_RAM  = 134;
static const int Y_FAN  = 186;   

static void drawPartial(DisplayType& display,
                        int16_t y,
                        const char* label,
                        const char* value)
{
    int16_t top = y - 18;
    if (top < 0) top = 0;

    display.setPartialWindow(0, top, 416, LINE_H);
    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);

        display.setCursor(X_LABEL, y);
        display.print(label);

        display.setCursor(X_VALUE, y);
        display.print(value);
    }
    while (display.nextPage());
}

void updateCPU(DisplayType& display, uint8_t value)
{
    xSemaphoreTake(displayMutex, portMAX_DELAY);

    char buf[16];
    snprintf(buf, sizeof(buf), "%d C", value);
    drawPartial(display, Y_CPU, "CPU:", buf);

    xSemaphoreGive(displayMutex);
}

void updateLOAD(DisplayType& display, uint8_t value)
{
    xSemaphoreTake(displayMutex, portMAX_DELAY);
    char buf[16];
    snprintf(buf,sizeof(buf), "%u%%", value);
    drawPartial(display, Y_LOAD, "LOAD:", buf);
    xSemaphoreGive(displayMutex);
}

void updateURAM(DisplayType& display, uint16_t uram, uint16_t tram)
{
    float u = uram / 10.0;
    float t = tram / 10.0;

    xSemaphoreTake(displayMutex, portMAX_DELAY);
    char buf[32];
    snprintf(buf, sizeof(buf), "%.1f / %.1f GB", u, t);
    drawPartial(display, Y_RAM,
                "RAM:",
                buf);
    xSemaphoreGive(displayMutex);
}

void updateFAN(DisplayType& display, uint8_t value)
{
    xSemaphoreTake(displayMutex, portMAX_DELAY);
    char buf[16];
    snprintf(buf, sizeof(buf), "%d RPM", value);
    drawPartial(display, Y_FAN, "FAN:", buf);
    xSemaphoreGive(displayMutex);
}