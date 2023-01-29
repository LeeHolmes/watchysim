#ifndef WATCHY_MACPAINT_H
#define WATCHY_MACPAINT_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM
#include "macpaint.h"

class WatchyMacPaint : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif