#ifndef WATCHY_NIOBE_H
#define WATCHY_NIOBE_H

#include "Watchy.h"

class Niobe : public Watchy {
public:
    Niobe();
    void drawWatchFace();

private:
    void drawCharWithRotation(const int charToDraw[], int x, int y, double rotation);
};
#endif