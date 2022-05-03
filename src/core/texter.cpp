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
		int r = s[i].height +  s[i].top;
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

static uint8_t *build_data (std::vector<Symbol> &s, int tw, int th, int font_size, int count)
{
	if (count == 0) count = 1;
	uint8_t *data = new uint8_t[tw * th * count];

	int size = s.size();
	uint8_t *z = data;
	uint8_t *szz = data;
	memset (z, 0x0, tw * th * count);
	int curline = 0;
	for (int i = 0; i < size; i++) {
		szz += s[i].left + s[i].width;
		if (s[i].line) {
			curline++;
			szz = &data[tw * font_size * curline];
			z = szz;
			szz += s[i].left + s[i].width;
		}
		z += s[i].left;
		for (int y = 0; y < th; y++) {
			int sz = s[i].width;
			if (s[i].min > y) {
				z += tw;
				continue;
			}
			int qw = tw;
			if (s[i].y >= s[i].height) {
				break;
			} else {
				int index = s[i].y * sz;
				for (int a = 0; a < sz; a++) {
					if (s[i].buf[index] > 0x0) {
						z[a] = s[i].buf[index];
					}
					index++;
				}
				s[i].y++;
			}
			z += tw;
		}
		z = szz;
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

Link *Texter::generate_link (wchar_t *message, int font_size, int width_constraint)
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
			is_space = 1;
		}
		if (is_space) {
			width = font_size;
			height = font_size;
			total = width * height;
			left = 0;
			top = font_size;
			sym.is_space = 1;
			sym.buf = new uint8_t[total];
			memset (sym.buf, 0x0, total);
		} else {
			sym.buf = new uint8_t[total];
			sym.is_space = 0;
			memcpy (sym.buf, bitmap.buffer, total);
		}
		sym.width = width;
		sym.top = top;
		sym.left = left;
		sym.height = height;
		sym.y = 0;
		sym.line = 0;

		syms.push_back (sym);

		pen_x += face->glyph->advance.x;
		pen_y += face->glyph->advance.y;
	}

	int count = 1;

	int wi = 0;
	int mi = 0;
	for (int i = 0; i < syms.size (); i++) {
		if (syms[i].is_space) {
			mi = i;
			wi += syms[i].width;
			for (int ii = i + 1; ii < syms.size (); ii++) {
				wi += syms[ii].width;
				if ((width_constraint > 0) && (wi >= width_constraint)) {
					syms[i + 1].line = 1;
					i++;
					count++;
					wi = 0;
					break;
				} else if (syms[ii].is_space) {
					i = ii - 1;
					break;
				}
			}
			if (i > mi) continue;
		} else {
			syms[i].line = 0;
			wi += syms[i].width;
		}
	}

	int max_width = width_constraint > 0 ? width_constraint : get_sym_max_width (syms);
	int max_height = get_sym_max_height (syms);
	int min_height = get_sym_min_height (syms);
	int min = 90;

	for (int i = 0; i < syms.size(); i++) {
		syms[i].offset = max_height - syms[i].height;
		syms[i].min = max_height - syms[i].top;
		if (min > syms[i].min) min = syms[i].min;
	}

	max_height += min;

	int total = max_width * max_height * count;
	uint8_t *alpha_data = build_data (syms, max_width, max_height, font_size, count);
	uint8_t *data = build_texture (alpha_data, max_width, max_height * count);
	Link *link = downloader_text_get_link (data, max_width, max_height * count);
	delete[] alpha_data;
	delete[] data;

	return link;
}
