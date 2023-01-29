#ifndef WATCHY_TETRIS_H
#define WATCHY_TETRIS_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
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