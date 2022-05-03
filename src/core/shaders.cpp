#ifdef __ANDROID__
#include "SDL.h"
#include <android/log.h>
#else
#include <SDL2/SDL.h>
#endif
#include <GLES3/gl3.h>
#include <stdint.h>
#include <iostream>
#include <core/shaders.h>
#include <core/globals.h>
#include <core/shader.h>
#include <shaders/shader-simple-2d-opengl.h>

extern Shader **global_shader;

static uint32_t program[TYPE_SHADERS_N];

static long get_total_file_size (SDL_RWops *io)
{
	SDL_RWseek (io, 0, RW_SEEK_END);
	long pos = SDL_RWtell (io);
	SDL_RWseek (io, 0, RW_SEEK_SET);
	return pos;
}

static char *get_data_from_file (const char *filename)
{
	SDL_RWops *io = SDL_RWFromFile (filename, "rb");
	long pos = get_total_file_size (io);
	char *data = new char[pos + 1];
	SDL_RWread (io, data, pos, 1);
	data[pos] = 0;
	SDL_RWclose (io);
	return data;
}

static uint32_t compile_shader (int type, const char *filename)
{
	char *data = get_data_from_file (filename);

	uint32_t shader = glCreateShader (type);
	glShaderSource (shader, 1, &data, nullptr);
	glCompileShader (shader);

	int status;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		int log_length;
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_length);
		char *log = new char[log_length];
		glGetShaderInfoLog (shader, log_length, nullptr, log);
		std::cout << log << "!!" << std::endl;
		delete []log;
	}

	return shader;
}

static uint32_t link_program (uint32_t vertex, uint32_t fragment)
{
	uint32_t program = glCreateProgram ();
	glAttachShader (program, vertex);
	glAttachShader (program, fragment);
	glLinkProgram (program);

	int status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		int log_length;
		glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_length);
		char *log = new char[log_length];
		glGetProgramInfoLog (program, log_length, nullptr, log);
		std::cout << log << "!!" << std::endl;
		delete []log;
	}

	return program;
}

static uint load_shader (const char *vert, const char *frag)
{
	uint32_t vertex = compile_shader (GL_VERTEX_SHADER, vert);
	uint32_t fragment = compile_shader (GL_FRAGMENT_SHADER, frag);
	uint32_t program = link_program (vertex, fragment);

	return program;
}

void init_shaders ()
{
	global_shader = new Shader *[TYPE_SHADERS_N];

	for (int i = 0; i < TYPE_SHADERS_N; i++) {
		switch (i) {
			case SHADER_MAIN: program[i] = load_shader (SHADER_CONVERT_TO_VERT (SHADER_MAIN), SHADER_CONVERT_TO_FRAG (SHADER_MAIN));
					  global_shader[i] = new ShaderSimple2DOpenGL ();
					  global_shader[i]->init (program[i]);
					  break;
		}
	}
}
