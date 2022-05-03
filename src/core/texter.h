#pragma once
#include <wchar.h>
#include <stdint.h>
#include <vector>
#include <ft2build.h>
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
		uint8_t *buf;
};

class Texter {
	public:
		FT_Library ft;
		FT_Face face;
		Texter ();
		~Texter ();
		Link *generate_link (wchar_t *message, int font_size);
};
