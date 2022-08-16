#include <iostream>
#include <GLES3/gl3.h>
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif
#include "to_download.h"
#include "link-opengl.h"
#include <stdio.h>

static Link *link[LINKS_N];

static void gen_vao_vbo (Link *link)
{
	static float v[18] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

#if 0
	static float t[12] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
#else
	static float t[12] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
#endif

	memcpy (link->v, v, 18 * sizeof (float));
	memcpy (link->t, t, 12 * sizeof (float));

}

static int swap_little_big_engian (int num)
{
	return (((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000));
}

static uint8_t **diff_file_to_textures (Link *link, const char *filename)
{
	int lb = 0;

	SDL_RWops *io = SDL_RWFromFile (filename, "rb");
	SDL_RWseek (io, 0, RW_SEEK_END);
	long pos = SDL_RWtell (io);
	SDL_RWseek (io, 0, RW_SEEK_SET);
	uint8_t *file = new uint8_t[pos];
	SDL_RWread (io, file, pos, 1);
	SDL_RWclose (io);

	const int LTBE = 0;
	const int COUNT = 1;
	const int WIDTH = 2;
	const int HEIGHT = 3;
	int *pack[4];

	for (int i = 0; i < 4; i++) {
		pack[i] = (int *) &file[i * 4];
	}

	if (*pack[LTBE] != 1) {
		for (int i = 1; i < 4; i++) {
			*pack[i] = swap_little_big_engian (*pack[i]);
		}
	}

	link->size_tex = *pack[COUNT];
	link->width = *pack[WIDTH];
	link->height = *pack[HEIGHT];


	uint8_t **data = new uint8_t*[link->size_tex];

	pos = 4 * sizeof (int);

	int total = link->width * link->height * 4;

	for (int i = 0; i < link->size_tex; i++) {
		data[i] = new uint8_t[total];
		memcpy (data[i], &file[pos], total);
		pos += total;
	}

	return data;
}

static void dump (const uint8_t *d)
{
	for ( int i = 0; i < 255; i++) {
		if (i > 0 && i % 16 == 0) {
			printf ("\n");
		}
		printf ("%02x ", d[i]);
	}
	printf ("\n");
}

static void gen_texture (Link *link, const char *filename)
{
	uint8_t **data = diff_file_to_textures (link, filename);

	link->tex = new uint32_t[link->size_tex];
	glGenTextures (link->size_tex, link->tex);

	for (int i = 0; i < link->size_tex; i++) {
		glBindTexture (GL_TEXTURE_2D, link->tex[i]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, link->width, link->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture (GL_TEXTURE_2D, 0);
		delete data[i];
	}
	delete data;
}

static Link *load_link (const char *filename) 
{
	Link *link = new Link ();
	gen_vao_vbo (link);
	gen_texture (link, filename);
	return link;
}

Link *downloader_load (const enum TO_DOWNLOADS file)
{
	switch (file) {
		case LINK_LOGO:
			if (link[LINK_LOGO] == nullptr)
				link[LINK_LOGO] = load_link (TO_STRING_FILENAME (LINK_LOGO));
			break;
		case LINK_SWAMP:
			if (link[LINK_SWAMP] == nullptr)
				link[LINK_SWAMP] = load_link (TO_STRING_FILENAME (LINK_SWAMP));
			break;
		case LINK_SUN:
			if (link[LINK_SUN] == nullptr)
				link[LINK_SUN] = load_link (TO_STRING_FILENAME (LINK_SUN));
			break;
		case LINK_CLOUDS:
			if (link[LINK_CLOUDS] == nullptr)
				link[LINK_CLOUDS] = load_link (TO_STRING_FILENAME (LINK_CLOUDS));
			break;
		case LINK_BOY:
			if (link[LINK_BOY] == nullptr)
				link[LINK_BOY] = load_link (TO_STRING_FILENAME (LINK_BOY));
			break;
		case LINK_FISH_LAY:
			if (link[LINK_FISH_LAY] == nullptr)
				link[LINK_FISH_LAY] = load_link (TO_STRING_FILENAME (LINK_FISH_LAY));
			break;
		case LINK_MAP:
			if (link[LINK_MAP] == nullptr)
				link[LINK_MAP] = load_link (TO_STRING_FILENAME (LINK_MAP));
			break;
		case LINK_HOME:
			if (link[LINK_HOME] == nullptr)
				link[LINK_HOME] = load_link (TO_STRING_FILENAME (LINK_HOME));
			break;
		case LINK_LETTER_FROM_POSTMAN:
			if (link[LINK_LETTER_FROM_POSTMAN] == nullptr)
				link[LINK_LETTER_FROM_POSTMAN] = load_link (TO_STRING_FILENAME (LINK_LETTER_FROM_POSTMAN));
			break;
		case LINK_TROFE:
			if (link[LINK_TROFE] == nullptr)
				link[LINK_TROFE] = load_link (TO_STRING_FILENAME (LINK_TROFE));
			break;
		case LINK_MESSAGE_WORMS:
			if (link[LINK_MESSAGE_WORMS] == nullptr)
				link[LINK_MESSAGE_WORMS] = load_link(TO_STRING_FILENAME(LINK_MESSAGE_WORMS));
			break;
		default:
			break;
	}

	return link[file];
}

static void gen_texture_from_buffer (Link *link, uint8_t *data, int max_width, int max_height)
{
	link->tex = new uint32_t[1];
	glGenTextures (1, link->tex);

	link->width = max_width;
	link->height = max_height;

	glBindTexture (GL_TEXTURE_2D, link->tex[0]);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, link->width, link->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Link *downloader_text_get_link (uint8_t *data, int max_width, int max_height)
{
	Link *link = new Link ();
	gen_vao_vbo (link);
	gen_texture_from_buffer (link, data, max_width, max_height);
	return link;
}

void downloader_free_all () {
	for (int i = 0; i < LINKS_N; i++) {
        if (link[i] != nullptr) {
            glDeleteTextures(link[i]->size_tex, link[i]->tex);
            link[i]->size_tex = 0;
            link[i] = nullptr;
        }

	}
}

void downloader_load_all () {
    for (int i = 0; i < LINKS_N; i++) {
        downloader_load((enum TO_DOWNLOADS) i);
    }
}