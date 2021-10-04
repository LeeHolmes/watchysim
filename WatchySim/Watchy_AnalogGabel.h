#ifndef WATCHY_MACPAINT_H
#define WATCHY_MACPAINT_H

#include "Watchy.h"
#include "analoggabel.h"

class WatchyAnalogGabel : public Watchy {
public:
    WatchyAnalogGabel();
    void drawWatchFace();

private:
    char* dayShortStr(uint8_t day);
};

#endif