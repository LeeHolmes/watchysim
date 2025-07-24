#ifdef WATCHY_SIM
    #ifdef _WIN32
        #include "..\..\Watchy.h"
    #else
        #include "../../Watchy_SDL2.h"
    #endif
#else // WATCHY_SIM
    #include <Watchy.h>
#endif // WATCHY_SIM

class WatchyDrawTest : public Watchy {
public:
    WatchyDrawTest();
    void drawWatchFace();
};