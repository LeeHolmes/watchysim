#ifndef WATCHY_MARIO_H
#define WATCHY_MARIO_H

#ifdef WATCHY_SIM
    #ifdef _WIN32
        #include "..\..\Watchy.h"
    #else
        #include "../../Watchy_SDL2.h"
    #endif
#else // WATCHY_SIM
    #include <Watchy.h>
#endif // WATCHY_SIM
#include "mario.h"

class WatchyMario: public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif