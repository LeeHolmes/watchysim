#ifndef WATCHY_7_SEG_H
#define WATCHY_7_SEG_H

#ifdef WATCHY_SIM
    #ifdef _WIN32
        #include "..\..\Watchy.h"
    #else
        #include "../../Watchy_SDL2.h"
    #endif
#else // WATCHY_SIM
    #include <Watchy.h>
#endif // WATCHY_SIM
#include "Seven_Segment10pt7b.h"
#include "DSEG7_Classic_Regular_15.h"
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Regular_39.h"
#include "icons.h"

class Watchy7SEG : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace() override;
        void handleButtonPress(uint8_t buttonID) override;
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawWeather();
        void drawBattery();
    private:
        int displayMode = 0; // 0: normal, 1: show seconds, 2: show date large
};

#endif