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
    void drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);

    void setContext(Graphics *graphics, HDC *hdc);

    void setTextColor(uint16_t color);
    void setFont(const GFXfont *f = NULL);
    void setCursor(int16_t x, int16_t y);
    void println(const char* text);

    void print(String text);
    void print(const char* text);
    void print(int number);
    void print(char text);

    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

private:
    void drawBitmapRaw(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, bool rasterMode);
    void drawFontBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);

    Graphics *graphics;
    HDC *hdc;

    uint16_t currentX;
    uint16_t currentY;
    uint16_t currentFontColor;
    const GFXfont *currentFont;
};

class Watchy {
public:
    DisplaySim display;
    tmElements_t currentTime;
public:
    Watchy();
    void showWatchFace(Graphics *graphics, HDC *hdc);
    void setTime(tm newTime);
    void resetTime();

    void init();
    virtual void drawWatchFace();
    float getBatteryVoltage();

private:
    Graphics *graphics;
};