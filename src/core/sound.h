#pragma once
#include <stdint.h>
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#define FORMAT_MONO    0
#define FORMAT_STEREO  1

class Sound {
public:
    virtual void init () = 0;
    virtual void set (char *file, int format, int freq) = 0;
    virtual void play () = 0;
    virtual void stop () = 0;
    virtual bool is_no_play () = 0;
};

static void *sound_get_data (char *file, size_t *size)
{
    SDL_RWops *io = SDL_RWFromFile (file, "rb");
    SDL_RWseek (io, 0, RW_SEEK_END);
    size_t pos = SDL_RWtell (io);
    SDL_RWseek (io, 0, RW_SEEK_SET);
    uint8_t *data = new uint8_t[pos];
    SDL_RWread (io, data, pos, 1);
    SDL_RWclose (io);
    *size = pos;
    return data;
}
