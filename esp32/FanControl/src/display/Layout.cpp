#include "Layout.h"
#include "DisplayContext.h"
//#include "luva.h"
#include "baku.h"
#include "assets/fan.h"


void drawStaticLayout(DisplayType& display)
{
    display.setFullWindow();

    display.firstPage();
    do
    {
        display.fillScreen(GxEPD_WHITE);
        
        display.setCursor(1, 20);
        display.print("AGUARDANDO CONEXAO...");
        
    
    } while (display.nextPage());
}
