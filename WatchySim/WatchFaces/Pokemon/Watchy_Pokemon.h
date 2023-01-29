#ifndef WATCHY_POKEMON_H
#define WATCHY_POKEMON_H

#ifdef WATCHY_SIM
#include "..\..\Watchy.h"
#else // WATCHY_SIM
#include <Watchy.h>
#endif // WATCHY_SIM
#include "pokemon.h"

class WatchyPokemon : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif