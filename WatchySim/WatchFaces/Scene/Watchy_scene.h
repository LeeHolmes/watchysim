#ifndef WATCHY_SCENE_H
#define WATCHY_SCENE_H

#ifdef WATCHY_SIM
    #ifdef _WIN32
        #include "..\..\Watchy.h"
    #else
        #include "../../Watchy_SDL2.h"
    #endif
#else // WATCHY_SIM
    #include <Watchy.h>
#endif // WATCHY_SIM

class Scene : public Watchy {
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};
#endif