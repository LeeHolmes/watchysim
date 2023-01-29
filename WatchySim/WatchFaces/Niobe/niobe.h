#ifndef WATCHY_NIOBE_H
#define WATCHY_NIOBE_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM

class Niobe : public Watchy {
    using Watchy::Watchy;
    public:
        void drawWatchFace();

    private:
        void drawCharWithRotation(const int charToDraw[], int x, int y, double rotation);
};
#endif