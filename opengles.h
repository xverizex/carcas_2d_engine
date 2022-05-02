#pragma once
#include "graphics.h"

class OpenGLES: public Graphics {
	public:
		OpenGLES ();
		SDL_GLContext init (SDL_Window *) override;
		void clear (uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;
		void render (SDL_Window *) override;
};
