#include "Watchy.h" //include the Watchy library
#include "analoggabel.h"
#include "FreeSerifBoldItalic9pt7b.h"
#include "FreeSerifItalic9pt7b.h"
#include "FreeSerifBold12pt7b.h"
#include "FreeSerifBoldItalic4pt7b.h"

class WatchyAnalogGabel : public Watchy { //inherit and extend Watchy class
public:
    void drawWatchFace() { //override this method to customize how the watch face looks

      //background
        display.fillScreen(GxEPD_BLACK);
        display.drawBitmap(0, 0, clockFace_square2, 200, 200, GxEPD_WHITE);
        display.fillCircle(100, 100, 3, GxEPD_WHITE);

        //Logo
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(80, 40);
        display.setFont(&FreeSerifBoldItalic9pt7b);
        display.print("Gabel ");
        display.setCursor(54, 60);
        display.setFont(&FreeSerifItalic9pt7b);
        display.print("Chronometrie");

        //date
        String dateDay = "";
        if (currentTime.Day < 10) {
            dateDay += "0";
        }

        char buffer[3];
        _itoa_s<3>(currentTime.Day, buffer, 10);

        dateDay += buffer;
        display.fillRect(128, 88, 27, 24, GxEPD_WHITE);
        display.setFont(&FreeSerifBold12pt7b);
        display.setTextColor(GxEPD_BLACK);
        display.setCursor(129, 107);
        display.print(dateDay);

        //weekday
        String wDay = dayShortStr(currentTime.Wday);
        display.fillRect(86, 140, 29, 22, GxEPD_WHITE);
        display.setFont(&FreeSerifBold12pt7b);
        display.setTextColor(GxEPD_BLACK);
        display.setCursor(87, 158);
        display.print(wDay);

        // draw battery
        display.drawCircleHelper(45, 100, 20, 2, GxEPD_WHITE);
        display.drawCircleHelper(45, 100, 20, 4, GxEPD_WHITE);
        display.drawPixel(65, 100, GxEPD_WHITE);
        display.drawFastVLine(45, 79, 4, GxEPD_WHITE);
        display.drawFastHLine(63, 100, 4, GxEPD_WHITE);
        display.drawFastVLine(45, 118, 4, GxEPD_WHITE);
        display.setFont(&FreeSerifBoldItalic4pt7b);
        display.setTextColor(GxEPD_WHITE);
        display.setCursor(44, 76);
        display.print("1");
        display.setCursor(70, 97);
        display.print("1");
        display.setCursor(69, 101);
        display.print("--");
        display.drawPixel(71, 100, GxEPD_WHITE);
        display.setCursor(69, 107);
        display.print("2");
        display.setCursor(43, 127);
        display.print("0");
        display.fillCircle(45, 100, 2, GxEPD_WHITE);
        double batteryCurrent = (getBatteryVoltage() - 3.3) / 0.9;
        double batteryAngle = batteryCurrent * 180;
        double radBattery = ((batteryAngle) * 71) / 4068.0;
        double bx1 = 45 + (sin(radBattery) * 16);
        double by1 = 100 + (cos(radBattery) * 16);
        display.drawLine(45, 100, (int)bx1, (int)by1, GxEPD_WHITE);

        //minute pointer
        int currentMinute = currentTime.Minute;
        int minuteAngle = currentMinute * 6;
        double radMinute = ((minuteAngle + 180) * 71) / 4068.0;
        double mx1 = 100 - (sin(radMinute) * 85);
        double my1 = 100 + (cos(radMinute) * 85);
        display.drawLine(100, 100, (int)mx1, (int)my1, GxEPD_WHITE);

        //hour pointer
        int currentHour = currentTime.Hour;
        double hourAngle = (currentHour * 30) + currentMinute * 0.5;
        double radHour = ((hourAngle + 180) * 71) / 4068.0;
        double hx1 = 100 - (sin(radHour) * 45);
        double hy1 = 100 + (cos(radHour) * 45);
        display.drawLine(100, 100, (int)hx1, (int)hy1, GxEPD_WHITE);
    }
};

WatchyAnalogGabel m; //instantiate your watchface

void setup() {
    m.init(); //call init in setup
}

void loop() {
    // this should never run, Watchy deep sleeps after init();
}