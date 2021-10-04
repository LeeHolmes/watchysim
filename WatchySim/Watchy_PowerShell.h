#ifndef WATCHY_POWERSHELL_H
#define WATCHY_POWERSHELL_H

#include "Watchy.h"
#include "gfxfont.h"
#include "powershell.h"

class WatchyPowerShell : public Watchy {
public:
    WatchyPowerShell();
    void drawWatchFace();
};

#endif