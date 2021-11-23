#include <ctime>

#include "Watchy.h"
#include "gfxfont.h"
#include "niobe.h"

Niobe::Niobe() {};

const int numbers[10][146] = {

    // 0
    {
    9, 16,
    0, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    1, 1, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    0, 0, 1, 1, 0, 1, 1, 1, 0,
    },

    // 1
    {
    9, 16,
    0, 0, 0, 1, 1, 1, 0, 0, 0,
    0, 0, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 1, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    },

    // 2
    {
    9, 16,
    0, 0, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    },

    // 3
    {
    9, 16,
    0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 
    0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    },

    // 4
    {
    9, 16,
    0, 0, 0, 0, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 1, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 1, 0, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 1, 1, 0, 0,
    1, 1, 0, 0, 0, 1, 1, 0, 0,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 
    },

    // 5
    {
    9, 16,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    },

    // 6
    {
    9, 16,
    0, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 0, 1, 1, 1, 0,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    },

    // 7
    {
    9, 16,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0,
    },

    // 8
    {
    9, 16,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    },

    // 9
    {
    9, 16,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 0,
    }
};

const int dot[] = {
    5, 6,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 1, 1, 1, 0,
};

const int bar[] = {
    6, 4,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1,
};

const int thin_bar[] = {
    6, 1,
    1, 0, 1, 0, 1, 1,
};

void Niobe::drawWatchFace() {
    display.fillScreen(GxEPD_BLACK);

    // Disk circles
    display.drawCircle(100, 100, 88, GxEPD_WHITE);
    display.drawCircle(100, 100, 100, GxEPD_WHITE);

    // Static Dots
    for (double deg = 0, step = 0; step < 12; deg += (2.0 * 3.1415 / 12.0), step++)
    {
        // Far outer dot
        drawCharWithRotation(dot, 96, 0, deg);

        // Hour minute separator dot
        drawCharWithRotation(dot, 63, 0, deg);

        // Before hour dot
        drawCharWithRotation(dot, 38, 0, deg);
    }

    // Thick outer bar
    for (double deg = 0, step = 0; step < 12; deg += (2.0 * 3.1415 / 12.0), step++)
    {
        drawCharWithRotation(bar, 89, 0, deg);
    }

    // Thin bars
    for (double deg = 0, step = 0; step < 60; deg += (2.0 * 3.1415 / 60.0), step++)
    {
        // Outer bezel bars
        drawCharWithRotation(thin_bar, 89, 0, deg);

        // Hour / minute separator bars
        drawCharWithRotation(thin_bar, 62, 0, deg);
    }

    // Minutes
    double minuteOffset = 2.0 * 3.1415 / 60.0 * currentTime.Minute;
    for (double deg = 0, step = 0; step < 12; deg += (2.0 * 3.1415 / 12.0), step++)
    {
        int minute = (int) step * 5;

        // seconds digit
        int second = minute % 10;
        drawCharWithRotation(numbers[second], 79, 0, deg - minuteOffset);

        // tens digit
        int tens = minute / 10;
        drawCharWithRotation(numbers[tens], 69, 0, deg - minuteOffset);
    }

    // Hours
    double hourOffset = (2.0 * 3.1415 / (60.0 * 12.0)) * (((currentTime.Hour - 1) % 12) * 60 + (currentTime.Minute));
    for (double deg = 0, step = 1; step < 13; deg += (2.0 * 3.1415 / 12.0), step++)
    {
        int hour = (int)step;

        // ones digit
        int ones = hour % 10;
        drawCharWithRotation(numbers[ones], 53, 0, deg - hourOffset);

        // tens digit
        int tens = hour / 10;
        if (tens > 0)
        {
            drawCharWithRotation(numbers[tens], 43, 0, deg - hourOffset);
        }
    }

    // Top of display window, double line
    display.drawFastHLine(100, 70, 100, GxEPD_WHITE);
    display.drawFastHLine(100, 71, 100, GxEPD_WHITE);
    
    // Bottom of display window, double line
    display.drawFastHLine(100, 129, 100, GxEPD_WHITE);
    display.drawFastHLine(100, 130, 100, GxEPD_WHITE);

    // Left half-circle of display window
    display.drawCircleHelper(100, 101, 30, 1, GxEPD_WHITE);
    display.drawCircleHelper(100, 100, 30, 8, GxEPD_WHITE);

    // Inner left half-circle of display window
    display.drawCircleHelper(132, 101, 19, 1, GxEPD_WHITE);
    display.drawCircleHelper(132, 100, 19, 8, GxEPD_WHITE);
    
    // Inner lines of display window
    display.drawFastHLine(135, 81, 65, GxEPD_WHITE);
    display.drawFastHLine(135, 119, 65, GxEPD_WHITE);

    // Clean up display window framing 
    display.fillRect(100, 72, 100, 9, GxEPD_BLACK);
    display.fillRect(100, 120, 100, 9, GxEPD_BLACK);

    // Display low batttery warning
    if (getBatteryVoltage() < 3.6)
    {
        display.fillCircle(195, 195, 5, GxEPD_WHITE);
    }
}

void Niobe::drawCharWithRotation(const int charToDraw[], int x, int y, double rotation)
{
    int width = charToDraw[0];
    int height = charToDraw[1];

    for (int xIter = 0; xIter < width; xIter++)
    {
        for (int yIter = 0; yIter < height; yIter++)
        {
            if (charToDraw[(yIter * width) + xIter + 2] == 1)
            {
                int draw_x = x + xIter;
                int draw_y = yIter - (height / 2);

                int screen_x = 100 + (int)(draw_x * cos(rotation) - draw_y * sin(rotation));
                int screen_y = 100 + (int)(draw_x * sin(rotation) + draw_y * cos(rotation));

                display.drawPixel(screen_x, screen_y, GxEPD_WHITE);
            }
        }
    }
}