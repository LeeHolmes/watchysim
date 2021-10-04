#include <ctime>
#include "Watchy_PowerShell.h"
#include "Px437_IBM_BIOS5pt7b.h"

WatchyPowerShell::WatchyPowerShell() {} //constructor

void WatchyPowerShell::drawWatchFace() {
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(0, 0, epd_bitmap_Watchy_PowerShell, 200, 200, GxEPD_WHITE);

    display.setTextColor(GxEPD_WHITE);
    display.setFont(&Px437_IBM_BIOS5pt7b);
    display.setCursor(3, 175);

    struct tm currentLocalTime;
    currentLocalTime.tm_wday = currentTime.Wday;
    currentLocalTime.tm_year = currentTime.Year + 1970 - 1900;
    currentLocalTime.tm_mon = currentTime.Month;
    currentLocalTime.tm_mday = currentTime.Day;
    currentLocalTime.tm_hour = currentTime.Hour;
    currentLocalTime.tm_min = currentTime.Minute;
    currentLocalTime.tm_sec = currentTime.Second;

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%a %b %d, %Y", &currentLocalTime);
    display.println(buffer);

    strftime(buffer, sizeof(buffer), "%I:%M %p", &currentLocalTime);
    display.print(buffer);
}