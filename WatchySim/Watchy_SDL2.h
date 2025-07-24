/*
 * Watchy_SDL2.h - Linux/SDL2 version of Watchy.h
 * 
 * This replaces the Windows-specific includes and types with
 * cross-platform alternatives.
 */

#ifndef WATCHY_SDL2_H
#define WATCHY_SDL2_H

#include <stdint.h>
#include <string>
#include <ctime>

#define PROGMEM

#include "gfxfont.h"
#include "FreeMonoBold9pt7b.h"
#include "arduino.h"
#include "arduino/String.h"
#include "config.h"
#include "DSEG7_Classic_Bold_53.h"
#include "GraphicsAdapter.h"

using namespace std;

#define GxEPD_WHITE 0xFFFF
#define GxEPD_BLACK 0x0000
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

#define DISPLAY_OFFSET_X 241
#define DISPLAY_OFFSET_Y 198

#define YEAR_OFFSET 1970

#define WEATHER_UPDATE_INTERVAL 30 //minutes

#define WATCHY_DEFAULT_TEMP 20
#define WATCHY_INTERNAL_TEMP 27

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
private:
    GraphicsContext* context;
    const GFXfont* currentFont;
    uint16_t currentFontColor;
    int16_t currentX, currentY;
    static const bool wrap = false;
    static const int16_t _width = DISPLAY_WIDTH;
    static const int16_t _height = DISPLAY_HEIGHT;
    
public:
    DisplaySim() : context(nullptr), currentFont(nullptr), currentFontColor(0), 
                   currentX(0), currentY(0) {}
    
    void fillScreen(uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);

    void setContext(GraphicsContext* ctx);

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

    // PIXELS
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void writePixel(int16_t x, int16_t y, uint16_t color);

    // LINES
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

    // WRITES
    void startWrite();
    void endWrite();

    // RECTS
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    // CIRCLES
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);

    // ROUNDED RECTS
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

private:
    void drawBitmapRaw(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, bool rasterMode);
    void drawFontBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);
    void charBounds(unsigned char c, int16_t *x, int16_t *y,
        int16_t *minx, int16_t *miny, int16_t *maxx,
        int16_t *maxy);
};

typedef struct weatherData {
    int8_t temperature;
    int16_t weatherConditionCode;
    bool isMetric;
    bool external;
} weatherData;

class Bma423 {
private:
    uint32_t stepCount;
public:
    Bma423() : stepCount(0) {}
    uint32_t getCounter() { return stepCount; }
    void setSteps(uint32_t steps) { stepCount = steps; }
    void resetStepCounter() { stepCount = 0; }
};

class Watchy {
public:
    DisplaySim display;
    tmElements_t currentTime;
    
    Watchy();
    void init();
    void setTime(tm t);
    void resetTime();
    void showWatchFace(GraphicsContext* context);
    virtual void drawWatchFace();
    virtual void handleButtonPress(uint8_t buttonID);
    
    float getBatteryVoltage();
    void setBatteryVoltage(float voltage);
    void setBluetooth(bool enabled);
    void setWifi(bool enabled);
    void setSteps(uint32_t stepCount);
    void setWeatherCode(int16_t code);
    void setWeatherExternal(bool external);
    void setTemperatureUnitMetric(bool isMetric);
    void setTemperature(int8_t temperature);
    int8_t getTemperature();
    
    static const char* dayStr(uint8_t day);
    static const char* dayShortStr(uint8_t day);
    static const char* monthShortStr(uint8_t month);
    
    weatherData getWeatherData();
    
protected:
    Bma423 sensor;
    bool WIFI_CONFIGURED;
    bool BLE_CONFIGURED;
    
private:
    float currentVoltage;
    weatherData currentWeather;
};

#endif // WATCHY_SDL2_H