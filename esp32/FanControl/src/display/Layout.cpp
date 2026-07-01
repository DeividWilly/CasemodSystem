#include "Layout.h"
#include "assets/wallpaper.h"


void drawStaticLayout(DisplayType& display)
{
    display.setFullWindow();

    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);

        display.drawBitmap(1, 1, epd_bitmap_images, 416, 240, GxEPD_BLACK);
        
    
    } while (display.nextPage());
}
