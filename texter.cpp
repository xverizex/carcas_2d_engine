#include "texter.h"
#include <string.h>
#include "downloader.h"
#ifdef __ANDROID__
#include "SDL.h"
#include <android/log.h>
#endif

static int dpii = 64;

Texter::~Texter ()
{
	FT_Done_FreeType (ft);
}

Texter::Texter ()
{
	FT_Init_FreeType (&ft);
#ifdef __ANDROID__
	SDL_RWops *io = SDL_RWFromFile ("terminus.ttf", "rb");
	SDL_RWseek (io, 0, RW_SEEK_END);
	long pos = SDL_RWtell (io);
	SDL_RWseek (io, 0, RW_SEEK_SET);
	uint8_t *data = new uint8_t[pos];
	SDL_RWread (io, data, pos, 1);
	SDL_RWclose (io);
	FT_Error err = FT_New_Memory_Face (ft, data, pos, 0, &face);
	delete[] data;
#else
	FT_New_Face (ft, "assets/terminus.ttf", 0, &face);
#endif
}

static int get_sym_max_width (std::vector<Symbol> &s)
{
	int max = 0;
	int size = s.size();
	for (int i = 0; i < size; i++) {
		int r = s[i].left + s[i].width;
		max += r;
	}

	return max;
}

static int get_sym_max_height (std::vector<Symbol> &s)
{
	int max = 0;
	int size = s.size();
	for (int i = 0; i < size; i++) {
		int r = s[i].height + s[i].height - s[i].top;
		if (max < r) max = r;
	}

	return max;
}

static int get_sym_min_height (std::vector<Symbol> &s)
{
	int min = 0;
	int size = s.size();
	for (int i = 0; i < size; i++) {
		int r = s[i].top;
		if (min < r) min = r;
	}

	return min;
}

static uint8_t *build_data (std::vector<Symbol> &s, int tw, int th)
{
	uint8_t *data = new uint8_t[tw * th];

	int size = s.size();
	uint8_t *z = data;
	memset (z, 0x0, tw * th);
	for (int y = 0; y < th; y++) {
		for (int i = 0; i < size; i++) {
			int sz = s[i].width;
			z += s[i].left;
#if 0
			if (s[i].offset > y) {
				memset (z, 0x0, sz);
				z += sz;
				continue;
			}
#endif
			if (s[i].min > y) {
				memset (z, 0x0, sz);
				z += sz;
				continue;
			}
			if (s[i].y >= s[i].height) 
				memset (z, 0x0, sz);
			else
				memcpy (z, &s[i].buf[s[i].y++ * sz], sz);
			z += sz;

		}
	}
	return data;
}

static uint8_t *build_texture (uint8_t *alpha, int max_width, int max_height)
{
	int total = max_width * max_height;
	uint8_t *data = new uint8_t[total * 4];
	int index = 0;
	for (int i = 0; i < total; i++) {
		data[index + 0] = 0x00;
		data[index + 1] = 0x00;
		data[index + 2] = 0x00;
		data[index + 3] = alpha[i];
		index += 4;
	}

	return data;
}

Link *Texter::generate_link (wchar_t *message, int font_size)
{
	FT_Set_Char_Size (face, 0, font_size * dpii, 96, 96);

	int load_flags = FT_LOAD_NO_BITMAP;
	FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL;

	int pen_x = 0;//1 * dpii;
	int pen_y = 0;//1 * dpii;

	std::vector<Symbol> syms;

	size_t len = wcslen (message);
	for (int i = 0; i < len; i++) {
		int is_space = 0;
		FT_UInt glyph = FT_Get_Char_Index (face, message[i]);
		FT_Error err = FT_Load_Glyph (face, glyph, load_flags);
		FT_Render_Glyph (face->glyph, render_mode);

		Symbol sym;
		FT_Bitmap bitmap = face->glyph->bitmap;
		int left = face->glyph->bitmap_left;
		int top = face->glyph->bitmap_top;

		int width = bitmap.width;
		int height = bitmap.rows;

		int total = width * height;
		if (total == 0) {
			width = font_size;
			height = font_size;
			total = width * height;
			is_space = 1;
		}
		sym.width = width;
		sym.top = top;
		sym.left = left;
		sym.height = height;
		sym.y = 0;
		sym.buf = new uint8_t[total];
		if (is_space)
			memset (sym.buf, 0x0, total);
		else
			memcpy (sym.buf, bitmap.buffer, total);

		syms.push_back (sym);

		pen_x += face->glyph->advance.x;
		pen_y += face->glyph->advance.y;
	}

	int max_width = get_sym_max_width (syms);
	int max_height = get_sym_max_height (syms);
	int min_height = get_sym_min_height (syms);
	int min = 90;

	for (int i = 0; i < syms.size(); i++) {
		syms[i].offset = max_height - syms[i].height;
		syms[i].min = max_height - syms[i].top;
		if (min > syms[i].min) min = syms[i].min;
	}

	max_height += min;

	int total = max_width * max_height;
	uint8_t *alpha_data = build_data (syms, max_width, max_height);
	uint8_t *data = build_texture (alpha_data, max_width, max_height);
	Link *link = downloader_text_get_link (data, max_width, max_height);
	delete[] alpha_data;
	delete[] data;

	return link;
}
