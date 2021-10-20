#include <ctime>
#include "Watchy.h"

Watchy::Watchy() {
    resetTime();
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
    return currentVoltage;
}

void Watchy::setBatteryVoltage(float voltage)
{
    currentVoltage = voltage;
}

void Watchy::setBluetooth(bool enabled)
{
    BLE_CONFIGURED = enabled;
}

void Watchy::setWifi(bool enabled)
{
    WIFI_CONFIGURED = enabled;
}

void Watchy::setSteps(uint32_t stepCount)
{
    sensor.setSteps(stepCount);
}

void Watchy::setWeatherCode(int16_t weatherConditionCode)
{
    currentWeather.weatherConditionCode = weatherConditionCode;
}

void Watchy::setTemperatureUnit(char* temperatureUnit)
{
    // If they are switching units, convert the temperature
    if (strcmp(TEMP_UNIT, temperatureUnit) != 0)
    {
        // Convert Celsius to Fahrenheit
        if (strcmp(temperatureUnit, "imperial") == 0)
        {
            currentWeather.temperature = (int8_t) (currentWeather.temperature * 9.0 / 5.0 + 32.0);
        }
        else
        {
            // Convert Fahrenheit to Celsius
            currentWeather.temperature = (int8_t) ((currentWeather.temperature - 32) * 5.0 / 9.0);
        }      
    }

    strncpy_s(TEMP_UNIT, temperatureUnit, 10);
}

void Watchy::setTemperature(int8_t temperature)
{
    if (strcmp(TEMP_UNIT, "imperial") == 0) {
        temperature = (int8_t) (temperature * 9.0 / 5.0 + 32.0); //fahrenheit
    }

    currentWeather.temperature = temperature;
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

void Watchy::resetTime()
{
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

weatherData Watchy::getWeatherData()
{
    return currentWeather;
}

const char* Watchy::dayShortStr(uint8_t day)
{
    const char *dayStr[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    return dayStr[day - 1];
}

const char* Watchy::dayStr(uint8_t day)
{
    const char *dayStr[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    return dayStr[day - 1];
}

const char* Watchy::monthShortStr(uint8_t month)
{
    const char *monthShortStr[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    return monthShortStr[month - 1];
}

void DisplaySim::setContext(Graphics *graphics, HDC *hdc)
{
    this->graphics = graphics;
    this->hdc = hdc;
}

void DisplaySim::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color)
{
    drawBitmapRaw(x, y, bitmap, w, h, color, false);
}

void DisplaySim::drawFontBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color)
{
    drawBitmapRaw(x, y, bitmap, w, h, color, true);
}

void DisplaySim::drawBitmapRaw(int16_t x, int16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, bool rasterMode)
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

void DisplaySim::println(String text)
{
    println(text.c_str());
}

void DisplaySim::println(const char* text)
{
    print(text);

    currentY += currentFont->yAdvance;
    currentX = 0;
}

void DisplaySim::println(char text)
{
    print(text);

    currentY += currentFont->yAdvance;
    currentX = 0;
}

void DisplaySim::println(uint8_t number)
{
    char buffer[_MAX_INT_DIG];
    _itoa_s<_MAX_INT_DIG>(number, buffer, 10);
    println(buffer);
}

void DisplaySim::println(uint32_t number)
{
    char buffer[_MAX_INT_DIG];
    _itoa_s<_MAX_INT_DIG>(number, buffer, 10);
    println(buffer);
}

void DisplaySim::println(int32_t number)
{
    char buffer[_MAX_INT_DIG];
    _itoa_s<_MAX_INT_DIG>(number, buffer, 10);
    println(buffer);
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

void DisplaySim::print(uint8_t number)
{
    char buffer[_MAX_INT_DIG];
    _itoa_s<_MAX_INT_DIG>(number, buffer, 10);
    print(buffer);
}

void DisplaySim::print(uint32_t number)
{
    char buffer[_MAX_INT_DIG];
    _itoa_s<_MAX_INT_DIG>(number, buffer, 10);
    print(buffer);
}

void DisplaySim::charBounds(unsigned char c, int16_t *x, int16_t *y,
    int16_t *minx, int16_t *miny, int16_t *maxx,
    int16_t *maxy) {

    if (currentFont) {

        if (c == '\n') { // Newline?
            *x = 0;        // Reset x to zero, advance y by one line
            *y += currentFont->yAdvance;
        }
        else if (c != '\r') { // Not a carriage return; is normal char
            uint8_t first = currentFont->first, last = currentFont->last;
            if ((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph glyph = currentFont->glyph[c - first];
                uint8_t gw = glyph.width, gh = glyph.height, xa = glyph.xAdvance;
                int8_t xo = glyph.xOffset, yo = glyph.yOffset;
                if (wrap && ((*x + (((int16_t)xo + gw))) > _width)) {
                    *x = 0; // Reset x to zero, advance y by one line
                    *y += (uint8_t) currentFont->yAdvance;
                }
                int16_t tsx = (int16_t)1, tsy = (int16_t)1,
                    x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + gw * tsx - 1,
                    y2 = y1 + gh * tsy - 1;
                if (x1 < *minx)
                    *minx = x1;
                if (y1 < *miny)
                    *miny = y1;
                if (x2 > *maxx)
                    *maxx = x2;
                if (y2 > *maxy)
                    *maxy = y2;
                *x += xa * tsx;
            }
        }

    }
    else { // Default font

        if (c == '\n') {        // Newline?
            *x = 0;               // Reset x to zero,
            *y += 8; // advance y one line
            // min/max x/y unchaged -- that waits for next 'normal' character
        }
        else if (c != '\r') { // Normal char; ignore carriage returns
            if (wrap && ((*x + 6) > _width)) { // Off right?
                *x = 0;                                       // Reset x to zero,
                *y += 8;                         // advance y one line
            }
            int x2 = *x + 6 - 1, // Lower-right pixel of char
                y2 = *y + 8 - 1;
            if (x2 > *maxx)
                *maxx = x2; // Track max x, y
            if (y2 > *maxy)
                *maxy = y2;
            if (*x < *minx)
                *minx = *x; // Track min x, y
            if (*y < *miny)
                *miny = *y;
            *x += 6; // Advance x one char
        }
    }
}

void DisplaySim::getTextBounds(String str, int16_t x, int16_t y,
    int16_t *x1, int16_t *y1, uint16_t *w,
    uint16_t *h)
{
    getTextBounds(str.c_str(), x, y, x1, y1, w, h);
}

void DisplaySim::getTextBounds(const char *str, int16_t x, int16_t y,
    int16_t *x1, int16_t *y1, uint16_t *w,
    uint16_t *h) {

    uint8_t c; // Current character
    int16_t minx = 0x7FFF, miny = 0x7FFF, maxx = -1, maxy = -1; // Bound rect
    // Bound rect is intentionally initialized inverted, so 1st char sets it

    *x1 = x; // Initial position is value passed in
    *y1 = y;
    *w = *h = 0; // Initial size is zero

    while ((c = *str++)) {
        // charBounds() modifies x/y to advance for each character,
        // and min/max x/y are updated to incrementally build bounding rect.
        charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);
    }

    if (maxx >= minx) {     // If legit string bounds were found...
        *x1 = minx;           // Update x1 to least X coord,
        *w = maxx - minx + 1; // And w to bound rect width
    }
    if (maxy >= miny) { // Same for height
        *y1 = miny;
        *h = maxy - miny + 1;
    }
}

void DisplaySim::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    SetPixel(*hdc, x + DISPLAY_OFFSET_X, y + DISPLAY_OFFSET_Y, color | color << 8);
}

void DisplaySim::writePixel(int16_t x, int16_t y, uint16_t color) {
    drawPixel(x, y, color);
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
    drawLine(x, y, x, y + h -1, color);
}

void DisplaySim::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    drawLine(x, y, x + w -1, y, color);
}

void DisplaySim::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawFastVLine(x, y, h, color);
}

void DisplaySim::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    drawFastHLine(x, y, w, color);
}

void DisplaySim::startWrite() {

}

void DisplaySim::endWrite() {

}

void DisplaySim::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    startWrite();
    writeFastHLine(x, y, w, color);
    writeFastHLine(x, y + h - 1, w, color);
    writeFastVLine(x, y, h, color);
    writeFastVLine(x + w - 1, y, h, color);
    endWrite();
}

void DisplaySim::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    startWrite();
    for (int16_t i = x; i < x + w; i++) {
        writeFastVLine(i, y, h, color);
    }
    endWrite();
}

void DisplaySim::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    fillRect(x, y, w, h, color);
}


void DisplaySim::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    startWrite();
    writePixel(x0, y0 + r, color);
    writePixel(x0, y0 - r, color);
    writePixel(x0 + r, y0, color);
    writePixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        writePixel(x0 + x, y0 + y, color);
        writePixel(x0 - x, y0 + y, color);
        writePixel(x0 + x, y0 - y, color);
        writePixel(x0 - x, y0 - y, color);
        writePixel(x0 + y, y0 + x, color);
        writePixel(x0 - y, y0 + x, color);
        writePixel(x0 + y, y0 - x, color);
        writePixel(x0 - y, y0 - x, color);
    }
    endWrite();
}

void DisplaySim::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            writePixel(x0 + x, y0 + y, color);
            writePixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            writePixel(x0 + x, y0 - y, color);
            writePixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            writePixel(x0 - y, y0 + x, color);
            writePixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            writePixel(x0 - y, y0 - x, color);
            writePixel(x0 - x, y0 - y, color);
        }
    }
}


void DisplaySim::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    startWrite();
    writeFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
    endWrite();
}

void DisplaySim::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color) {

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;

    delta++; // Avoid some +1's in the loop

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if (x < (y + 1)) {
            if (corners & 1)
                writeFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2)
                writeFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
        }
        if (y != py) {
            if (corners & 1)
                writeFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2)
                writeFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}

void DisplaySim::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
    // smarter version
    startWrite();
    writeFastHLine(x + r, y, w - 2 * r, color);         // Top
    writeFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    writeFastVLine(x, y + r, h - 2 * r, color);         // Left
    writeFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
    endWrite();
}

void DisplaySim::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius)
        r = max_radius;
    // smarter version
    startWrite();
    writeFillRect(x + r, y, w - 2 * r, h, color);
    // draw four corners
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
    endWrite();
}

void DisplaySim::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

void DisplaySim::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1);
        _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1);
        _swap_int16_t(x0, x1);
    }

    startWrite();
    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        writeFastHLine(a, y0, b - a + 1, color);
        endWrite();
        return;
    }

    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
        dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1; // Include y1 scanline
    else
        last = y1 - 1; // Skip it

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            _swap_int16_t(a, b);
        writeFastHLine(a, y, b - a + 1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            _swap_int16_t(a, b);
        writeFastHLine(a, y, b - a + 1, color);
    }
    endWrite();
}


uint32_t SensorSim::getCounter()
{
    return stepCount;
}

void SensorSim::resetStepCounter()
{
    stepCount = 0;
}

void SensorSim::setSteps(uint32_t stepCount)
{
    this->stepCount = stepCount;
}