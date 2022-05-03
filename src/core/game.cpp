#include "globals.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include "opengles.h"
#include <levels/levels.h>
#include "ilevel.h"
#include <levels/level-logo.h>
#include <queue>
#include "languages.h"

std::queue<struct event *> events;

static int static_sdl_thread (void *ptr)
{
	SDL_Event event;

	struct event *ev = new struct event ();

	while (SDL_WaitEvent (&event)) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				{
					SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent *) &event;
					ev->type = BUTTON_DOWN;
					ev->x = m->x;
					ev->y = m->y;
					events.push (ev);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				{
					SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent *) &event;
					ev->type = BUTTON_UP;
					ev->x = m->x;
					ev->y = m->y;
					events.push (ev);
				}
				break;
			default:
				continue;
		}

		ev = new struct event ();
	}

	return 0;
}

void Game::init ()
{
	SDL_Init (SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
#ifdef __ANDROID__
	SDL_DisplayMode display_mode;
	SDL_GetDisplayMode (0, 0, &display_mode);
	global_width = (float) display_mode.w;
	global_height = (float) display_mode.h;

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
#else
	global_width = 1080.0f;
	global_height = 2340.0f;

	int flags = SDL_WINDOW_OPENGL;
#endif

	global_aspect = global_height / global_width;

	SDL_DisableScreenSaver ();

	graphics = new OpenGLES ();

	this->win = SDL_CreateWindow ("one_in_taiga",
			0, 0,
			global_width,
			global_height,
			flags
			);

	this->ctx = graphics->init (this->win);

	SDL_CreateThread (static_sdl_thread, "static_sdl_thread", nullptr);
}

void Game::loop ()
{

	level = new ILevel *[LEVEL_N];
	level[LEVEL_LOGO] = new LevelLogo ();

	global_cur_level = LEVEL_LOGO;

	level[global_cur_level]->load ();

	while (1) {
		const uint8_t *key = SDL_GetKeyboardState (nullptr);
		if (key[SDL_SCANCODE_Q]) exit (0);

		level[global_cur_level]->clear_screen (graphics);

		struct event *ev = nullptr;

		if (!events.empty()) {
			ev = events.front ();
			events.pop ();
			handle_event (ev);
			delete ev;
			ev = nullptr;
		}
		
		level[global_cur_level]->render ();

		graphics->render (this->win);
		SDL_Delay (16);
	}
}

void Game::handle_event (struct event *ev)
{
	switch (ev->type) {
		case BUTTON_DOWN:
		case BUTTON_UP:
			level[global_cur_level]->handle_button (ev->type, ev->x, ev->y);
			break;
	}
}
