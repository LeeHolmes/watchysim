#ifndef WATCHY_DOS_H
#define WATCHY_DOS_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM
#include "Px437_IBM_BIOS5pt7b.h"

class WatchyDOS : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif