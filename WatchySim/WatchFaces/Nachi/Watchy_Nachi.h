#ifndef WATCHY_NACHI_H
#define WATCHY_NACHI_H

#ifdef WATCHY_SIM
    #ifdef _WIN32
        #include "..\..\Watchy.h"
    #else
        #include "../../Watchy_SDL2.h"
    #endif
#else // WATCHY_SIM
    #include <Watchy.h>
#endif // WATCHY_SIM
#include "../7_SEG/Seven_Segment10pt7b.h"
#include "../7_SEG/DSEG7_Classic_Regular_15.h"
#include "../7_SEG/DSEG7_Classic_Bold_25.h"
#include "../7_SEG/DSEG7_Classic_Regular_39.h"
#include "../7_SEG/icons.h"

class WatchyNachi : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace() override;
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawWeather();
        void drawBattery();
        void drawDualTemperature();
};

#endif