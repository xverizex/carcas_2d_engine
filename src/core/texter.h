#pragma once
#include <wchar.h>
#include <stdint.h>
#include <vector>
#ifdef __ANDROID__
#include <ft2build.h>
#else
#include <ft2build.h>
#endif
#include FT_FREETYPE_H
#include "link.h"

class Symbol {
	public:
		int width;
		int height;
		int left;
		int top;
		int y;
		int offset;
		int min;
		int is_space;
		int line;
		uint8_t *buf;
};

class Texter {
	public:
		FT_Library ft;
		FT_Face face;
		Texter ();
		~Texter ();
		Link *generate_link (wchar_t *message, int font_size, int width_constraint);
};
