#ifndef WATCHY_TETRIS_H
#define WATCHY_TETRIS_H

#ifdef WATCHY_SIM
    #ifdef _WIN32
        #include "..\..\Watchy.h"
    #else
        #include "../../Watchy_SDL2.h"
    #endif
#else // WATCHY_SIM
    #include <Watchy.h>
#endif // WATCHY_SIM
#include "tetris.h"

class WatchyTetris : public Watchy{
    public:
        using Watchy::Watchy;
        void drawWatchFace();
};

#endif