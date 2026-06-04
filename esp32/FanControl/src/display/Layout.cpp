#include "Layout.h"

void drawStaticLayout(DisplayType& display)
{
    display.setFullWindow();

    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);

        display.setCursor(10, 40);
        display.print("CPU:");

        display.setCursor(10, 80);
        display.print("LOAD:");

        display.setCursor(10, 120);
        display.print("RAM:");

        display.setCursor(10, 160);
        display.print("FAN:");

    } while (display.nextPage());
}