#pragma once
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif
#include <stdint.h>

class Graphics {
	public:
		virtual SDL_GLContext init (SDL_Window *win) = 0;
		virtual void clear (uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
		virtual void render (SDL_Window *win) = 0;
};
