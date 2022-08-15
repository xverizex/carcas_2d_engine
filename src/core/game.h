#pragma once
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif
#include "graphics.h"
#include "ilevel.h"

class Game {
	public:
		void init ();
		void loop ();
		void handle_event (struct event *);
		SDL_Window *win;
		SDL_GLContext ctx;

		Graphics *graphics;
		ILevel **level;
};
