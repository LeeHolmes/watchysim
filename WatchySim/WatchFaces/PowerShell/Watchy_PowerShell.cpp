#include <ctime>

#include "Watchy_Powershell.h"

WatchyPowerShell::WatchyPowerShell() {};

void WatchyPowerShell::drawWatchFace() {
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(0, 0, epd_bitmap_Watchy_PowerShell, 200, 200, GxEPD_WHITE);
    
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&Px437_IBM_BIOS5pt7b);
    display.setCursor(0, 175);
    
    struct tm currentLocalTime;
    currentLocalTime.tm_wday = currentTime.Wday - 1;
    currentLocalTime.tm_year = currentTime.Year + 1970 - 1900;
    currentLocalTime.tm_mon = currentTime.Month - 1;
    currentLocalTime.tm_mday = currentTime.Day;
    currentLocalTime.tm_hour = currentTime.Hour;
    currentLocalTime.tm_min = currentTime.Minute;
    currentLocalTime.tm_sec = currentTime.Second;
    
    char buffer[20];
    strftime(buffer, sizeof(buffer), " %a %b %d, %Y", &currentLocalTime);
    display.println(buffer);
    
    strftime(buffer, sizeof(buffer), " %I:%M %p", &currentLocalTime);
    display.print(buffer);

    // Show a low battery warning if appropriate
    float battery = getBatteryVoltage();
    if (battery < 3.80)
    {
        display.setCursor(170, 185);
        display.print("BAT");
    }
}
