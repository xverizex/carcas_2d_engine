#include "globals.h"
#include "game.h"
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif
#include <GLES3/gl3.h>
#include "opengles.h"
#include <levels/levels.h>
#include "ilevel.h"
#include <levels/level-logo.h>
#include <levels/level-swamp.h>
#include <levels/level-map.h>
#include <levels/level-home.h>
#include <queue>
#include "languages.h"

std::queue<struct event *> events;

static int static_sdl_thread (void *ptr)
{
	SDL_Event event;

	Game *game = static_cast<Game *>(ptr);

	struct event ev;

	while (SDL_WaitEvent (&event)) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				{
					SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent *) &event;
					ev.type = BUTTON_DOWN;
					ev.x = m->x;
					ev.y = m->y;
					//game->level[global_cur_level]->handle_button (ev->type, ev->x, ev->y);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				{
					SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent *) &event;
					ev.type = BUTTON_UP;
					ev.x = m->x;
					ev.y = m->y;
					game->level[global_cur_level]->handle_button (ev.type, ev.x, ev.y);
				}
				break;
			default:
				continue;
		}

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
	global_width = 3840.f;
	global_height = 2160.f;

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
#endif

	global_aspect = global_height / global_width;

	SDL_DisableScreenSaver ();

	graphics = new OpenGLES ();

	this->win = SDL_CreateWindow ("fishing boy",
			0, 0,
			global_width,
			global_height,
			flags
			);

	this->ctx = graphics->init (this->win);

	SDL_CreateThread (static_sdl_thread, "static_sdl_thread", this);
}

int global_power = 2;
int global_power_width = 2;
#include <logic/state.h>
#ifdef __ANDROID__
#include <jni.h>

JNIEnv *global_env;
#endif

extern StateGame state_game;


void Game::loop ()
{
#ifdef __ANDROID__
	global_env = static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv());
#endif

	int height = 270;
	int width = 480;
	while (height < global_height) {
		height *= 2;
		global_power++;
	}

	global_power_width = global_power;

	state_game.load ();

#if 0
	while (width < global_height) {
		width *= 2;
		global_power_width++;
	}
#endif

	level = new ILevel *[LEVEL_N];
	level[LEVEL_LOGO] = new LevelLogo ();
	level[LEVEL_SWAMP] = new LevelSwamp ();
	level[LEVEL_MAP] = new LevelMap ();
	level[LEVEL_HOME] = new LevelHome ();


#if 1
	for (int i = 0; i < LEVEL_N; i++) {
		level[i]->load ();
	}
#endif

    global_cur_level = LEVEL_LOGO;

//	level[global_cur_level]->load ();

	while (1) {
		ILevel *lvl = level[global_cur_level];

		const uint8_t *key = SDL_GetKeyboardState (nullptr);
		if (key[SDL_SCANCODE_Q]) exit (0);

		lvl->clear_screen (graphics);

		struct event *ev = nullptr;

		
		lvl->render ();

		if (lvl->from != lvl->to) {
			int level = lvl->to;
			lvl->unload ();
			global_cur_level = level;
	//		level[global_cur_level]->load ();
			continue;
		}

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
