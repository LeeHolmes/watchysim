#ifndef WATCHY_H
#define WATCHY_H

#include <stdint.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <string>

#define PROGMEM
#define String string

#include "gfxfont.h"
#include "FreeMonoBold9pt7b.h"

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

#define GxEPD_WHITE 0xFFFF
#define GxEPD_BLACK 0x0000
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

#define DISPLAY_OFFSET_X 241
#define DISPLAY_OFFSET_Y 198

#define YEAR_OFFSET 1970

#define WEATHER_UPDATE_INTERVAL 30 //minutes

typedef struct {
    uint8_t Second;
    uint8_t Minute;
    uint8_t Hour;
    uint8_t Wday;   // day of week, sunday is day 1
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;   // offset from 1970; 
} tmElements_t;

class DisplaySim {
public:
    void fillScreen(uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);

    void setContext(Graphics *graphics, HDC *hdc);

    void setTextColor(uint16_t color);
    void setFont(const GFXfont *f = NULL);
    void setCursor(int16_t x, int16_t y);
    
    void println(String text);
    void println(const char* text);
    void println(uint8_t number);
    void println(uint32_t number);
    void println(int32_t number);
    void println(char text);

    void print(String text);
    void print(const char* text);
    void print(uint8_t number);
    void print(uint32_t number);
    void print(char text);

    void getTextBounds(String str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w,
        uint16_t *h);

    void getTextBounds(const char *str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w,
        uint16_t *h);

    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

private:
    void drawBitmapRaw(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, bool rasterMode);
    void drawFontBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);

    void charBounds(unsigned char c, int16_t *x, int16_t *y,
        int16_t *minx, int16_t *miny, int16_t *maxx,
        int16_t *maxy);


    Graphics *graphics;
    HDC *hdc;

    int16_t currentX;
    int16_t currentY;
    uint16_t currentFontColor;
    const GFXfont *currentFont;

    uint16_t _width = DISPLAY_WIDTH;
    uint16_t _height = DISPLAY_HEIGHT;
    bool wrap = true;
};

typedef struct weatherData {
    int8_t temperature;
    int16_t weatherConditionCode;
} weatherData;

class SensorSim {
public:
    uint32_t getCounter();
    void resetStepCounter();

    void setSteps(uint32_t stepCount);

private:
    uint32_t stepCount = 5280;
};

class Watchy {
public:
    DisplaySim display;
    SensorSim sensor;
    tmElements_t currentTime;
public:
    Watchy();
    void showWatchFace(Graphics *graphics, HDC *hdc);
    void setTime(tm newTime);
    void resetTime();

    void init();
    virtual void drawWatchFace();
    float getBatteryVoltage();
    weatherData getWeatherData();

    void setBatteryVoltage(float newVoltage);
    void setBluetooth(bool enabled);
    void setWifi(bool enabled);
    void setSteps(uint32_t stepCount);
    void setWeatherCode(int16_t weatherConditionCode);
    
    void setTemperatureUnit(char* temperatureUnit);
    void setTemperature(int8_t temperature);

protected:
    const char* dayShortStr(uint8_t day);
    const char* dayStr(uint8_t day);
    
    const char* monthShortStr(uint8_t month);

    bool BLE_CONFIGURED = true;
    bool WIFI_CONFIGURED = true;
    char TEMP_UNIT[10] = "imperial";
    
private:
    Graphics *graphics;
    float currentVoltage = 3.96f;
    weatherData currentWeather = { 65, 550 };
};
#endif