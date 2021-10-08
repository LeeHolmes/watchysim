#include <ctime>
#include "Watchy.h"

Watchy::Watchy() {
    time_t curr_time;
    curr_time = time(NULL);

    struct tm tm_local;
    localtime_s(&tm_local, &curr_time);
    currentTime.Wday = tm_local.tm_wday + 1;
    currentTime.Day = tm_local.tm_mday;
    currentTime.Month = tm_local.tm_mon + 1;
    currentTime.Year = tm_local.tm_year + 1900 - 1970;
    currentTime.Hour = tm_local.tm_hour;
    currentTime.Minute = tm_local.tm_min;
    currentTime.Second = tm_local.tm_sec;
}

void Watchy::drawWatchFace() {}

void Watchy::init() {}

void Watchy::showWatchFace(Graphics *graphics, HDC *hdc)
{
    display.setContext(graphics, hdc);
    drawWatchFace();
}

float Watchy::getBatteryVoltage()
{
    return 3.96f;
}

void Watchy::setTime(tm newTime)
{
    currentTime.Wday = newTime.tm_wday + 1;
    currentTime.Day = newTime.tm_mday;
    currentTime.Month = newTime.tm_mon + 1;
    currentTime.Year = newTime.tm_year + 1900 - 1970;
    currentTime.Hour = newTime.tm_hour;
    currentTime.Minute = newTime.tm_min;
    currentTime.Second = newTime.tm_sec;
}

void DisplaySim::setContext(Graphics *graphics, HDC *hdc)
{
    this->graphics = graphics;
    this->hdc = hdc;
}

void DisplaySim::drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color)
{
    drawBitmapRaw(x, y, bitmap, w, h, color, false);
}

void DisplaySim::drawFontBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color)
{
    drawBitmapRaw(x, y, bitmap, w, h, color, true);
}

void DisplaySim::drawBitmapRaw(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, bool rasterMode)
{
    int bitmapOffset = 0;
    if (!rasterMode)
    {
        bitmapOffset--;
    }

    int bitOffset = 128;

    for (int currentY = 0; currentY < h; currentY++)
    {
        for (int currentX = 0; currentX < w; currentX++, bitOffset = bitOffset >> 1)
        {
            if (
                (rasterMode && (bitOffset == 0)) ||
                (!rasterMode && ((currentX % 8) == 0)))
            {
                bitOffset = 128;
                bitmapOffset++;
            }

            if (bitmap[bitmapOffset] & bitOffset)
            {
                SetPixel(*hdc,
                    x + currentX + DISPLAY_OFFSET_X,
                    y + currentY + DISPLAY_OFFSET_Y, color | color << 8);
            }
        }
    }
}

void DisplaySim::fillScreen(uint16_t color)
{
    Color c = color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255);
    SolidBrush fillColor(c);

    graphics->FillRectangle(&fillColor, DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void DisplaySim::setTextColor(uint16_t color)
{
    currentFontColor = color;
}

void DisplaySim::setFont(const GFXfont *f)
{
    currentFont = f;
}

void DisplaySim::setCursor(int16_t x, int16_t y)
{
    currentX = x;
    currentY = y;
}

void DisplaySim::println(const char* text)
{
    print(text);

    currentY += currentFont->yAdvance;
    currentX = 0;
}

void DisplaySim::print(String text)
{
    print(text.c_str());
}

void DisplaySim::print(const char* text)
{
    for (size_t currentChar = 0; currentChar < strlen(text); currentChar++)
    {
        char itemToPrint = text[currentChar];
        print(itemToPrint);
    }
}

void DisplaySim::print(char text)
{
    int fontOffset = text - currentFont->first;

    GFXglyph currentGlyph = currentFont->glyph[fontOffset];
    int bitmapOffset = currentGlyph.bitmapOffset;

    drawFontBitmap(currentX + currentGlyph.xOffset, currentY + currentGlyph.yOffset,
        &currentFont->bitmap[bitmapOffset], currentGlyph.width, currentGlyph.height, currentFontColor);
    currentX += currentGlyph.xAdvance;
}

void DisplaySim::print(int number)
{
    char buffer[_MAX_INT_DIG];
    _itoa_s<_MAX_INT_DIG>(number, buffer, 10);
    print(buffer);
}

void DisplaySim::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
    REAL startAngle = 0;

    if (cornername & 0x4) {
        startAngle = 0;
    }
    if (cornername & 0x2) {
        startAngle = -90;
    }
    if (cornername & 0x8) {
        startAngle = -180;
    }
    if (cornername & 0x1) {
        startAngle = -270;
    }
    REAL sweepAngle = 90;

    Color c = color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255);
    Pen fillColor(c);

    graphics->DrawArc(&fillColor, x0 + DISPLAY_OFFSET_X - r, y0 + DISPLAY_OFFSET_Y - r,
        r * 2, r * 2, startAngle, sweepAngle);

}

void DisplaySim::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    SetPixel(*hdc, x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y, color | color << 8);
}

void DisplaySim::fillCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    Color c = color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255);
    SolidBrush fillColor(c);

    graphics->FillEllipse(&fillColor, x + DISPLAY_OFFSET_X - r, y + DISPLAY_OFFSET_Y - r, 2*r, 2*r);

}

void DisplaySim::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    Color c = color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255);
    Pen fillColor(c);

    graphics->DrawRectangle(&fillColor, x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y, w, h);

}

void DisplaySim::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    Color c = color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255);
    SolidBrush fillColor(c);

    graphics->FillRectangle(&fillColor, x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y, w, h);

}

void DisplaySim::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    Color c = color == GxEPD_BLACK ? Color(255,0,0,0) : Color(255,255,255,255);
    Pen fillColor(c);

    graphics->DrawLine(&fillColor, x0 + DISPLAY_OFFSET_X, y0 + DISPLAY_OFFSET_Y,
        x1 + DISPLAY_OFFSET_X, y1 + DISPLAY_OFFSET_Y);
}

void DisplaySim::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    drawLine(x, y, x, y + h, color);
}

void DisplaySim::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    drawLine(x, y, x + w, y, color);
}