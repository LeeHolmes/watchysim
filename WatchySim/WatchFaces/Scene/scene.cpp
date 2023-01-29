#include <ctime>

#include "Watchy_scene.h"
#include "scene.h"
#include "ModernDOS8x168pt7b.h"

void Scene::drawWatchFace() {
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(2, 5, epd_bitmap_scene_digits[currentTime.Hour / 10], 48, 128, GxEPD_WHITE);
    display.drawBitmap(50, 0, epd_bitmap_scene_digits[currentTime.Hour % 10], 48, 128, GxEPD_WHITE);
    display.drawBitmap(102, 4, epd_bitmap_scene_digits[currentTime.Minute / 10], 48, 128, GxEPD_WHITE);
    display.drawBitmap(150, 2, epd_bitmap_scene_digits[currentTime.Minute % 10], 48, 128, GxEPD_WHITE);

    display.setTextColor(GxEPD_WHITE);
    display.setFont(&ModernDOS8x168pt7b);
        
    display.setCursor(0, 145);
    display.println("  =Dual 14.4k V.42bis!=");
    display.println("/-_-*-_-*-_-*-_-*-_-*-_-\\");
    display.print("| Last Login ");
    
    char buffer[11];
    snprintf(buffer, 11, "%02d/%02d/%d", currentTime.Month, currentTime.Day, currentTime.Year + 1970);
    display.print(buffer);
    display.println(" |");

    // Show a low battery warning if appropriate
    float battery = getBatteryVoltage();
    if (battery < 3.80)
    {
        display.println("\\._*= greetz SQFMI! =*_./");
    }
    else
    {
        display.println("\\._*= greetz @SQFMI =*_./");
    }
}
