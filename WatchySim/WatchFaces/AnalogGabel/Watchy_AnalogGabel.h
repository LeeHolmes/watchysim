#ifndef WATCHY_ANALOGGABEL_H
#define WATCHY_ANALOGGABELT_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM
#include "analoggabel.h"
#include "FreeSerifBoldItalic9pt7b.h"
#include "FreeSerifItalic9pt7b.h"
#include "FreeSerifBold12pt7b.h"
#include "FreeSerifBoldItalic4pt7b.h"

class WatchyAnalogGabel : public Watchy {
    using Watchy::Watchy;
public:
    void drawWatchFace();
};

#endif