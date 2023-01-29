#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM

class WatchyDrawTest : public Watchy {
public:
    WatchyDrawTest();
    void drawWatchFace();
};