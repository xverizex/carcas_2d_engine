#pragma once
#include <SDL2/SDL.h>
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
