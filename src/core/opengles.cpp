#include "opengles.h"
#include "globals.h"
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include "shaders.h"

OpenGLES::OpenGLES ()
{
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
}

SDL_GLContext OpenGLES::init (SDL_Window *win)
{
	SDL_GLContext ctx = SDL_GL_CreateContext (win);

        init_shaders ();

	glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glViewport (0, 0, global_width, global_height);

	return ctx;
}

void OpenGLES::clear (uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	glClearColor (r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLES::render (SDL_Window *win)
{
	SDL_GL_SwapWindow (win);
}
