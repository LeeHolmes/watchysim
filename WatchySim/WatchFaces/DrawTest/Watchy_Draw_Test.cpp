#include "Watchy_Draw_Test.h"

WatchyDrawTest::WatchyDrawTest() {} //constructor

void WatchyDrawTest::drawWatchFace() {
    display.fillScreen(GxEPD_BLACK);

    
    display.fillRect(0, 0, 200, 100, GxEPD_BLACK);
    display.fillRect(0, 100, 200, 100, GxEPD_WHITE);

    display.drawLine(100, 0, 100, 100, GxEPD_WHITE);
    display.drawLine(100, 100, 100, 200, GxEPD_BLACK);

    display.drawRect(2, 2, 30, 20, GxEPD_WHITE);
    display.drawRect(2, 102, 30, 20, GxEPD_BLACK);

    display.fillRect(102, 2, 30, 20, GxEPD_WHITE);
    display.fillRect(102, 102, 30, 20, GxEPD_BLACK);

    display.drawCircle(20, 40, 10, GxEPD_WHITE);
    display.drawCircle(20, 140, 10, GxEPD_BLACK);

    display.fillCircle(120, 40, 10, GxEPD_WHITE);
    display.fillCircle(120, 140, 10, GxEPD_BLACK);

    display.drawRoundRect(2, 60, 50, 30, 10, GxEPD_WHITE);
    display.drawRoundRect(2, 160, 50, 30, 10, GxEPD_BLACK);

    display.fillRoundRect(102, 60, 50, 30, 10, GxEPD_WHITE);
    display.fillRoundRect(102, 160, 50, 30, 10, GxEPD_BLACK);

    display.drawTriangle(73, 2, 98, 27, 48, 27, GxEPD_WHITE);
    display.drawTriangle(73, 102, 98, 127, 48, 127, GxEPD_BLACK);

    display.fillTriangle(173, 2, 198, 27, 148, 27, GxEPD_WHITE);
    display.fillTriangle(173, 102, 198, 127, 148, 127, GxEPD_BLACK);



}
