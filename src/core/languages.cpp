#include "languages.h"
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif
#include <string.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <clocale>

Language::Language ()
{
	std::setlocale (LC_ALL, "");
	char **file = init_list_lang ();

	for (int i = 0; i < LANG_N; i++) {
		wchar_t **text = read_and_convert_to_wchar (file[i]);
		langs[i] = text;
	}

	cur_lang = langs[0];
}

wchar_t **Language::read_and_convert_to_wchar (const char *file)
{
	SDL_RWops *io = SDL_RWFromFile (file, "r");
	SDL_RWseek (io, 0, RW_SEEK_END);
	long pos = SDL_RWtell (io);
	SDL_RWseek (io, 0, RW_SEEK_SET);

	char *data = new char[pos];
	SDL_RWread (io, data, pos, 1);
	SDL_RWclose (io);

	int count = 0;
	char *st = data;

	std::vector<wchar_t *> all_lines;

	for (int i = 0; i < pos; i++) {
		if (data[i] == '\n') {
			char *line = new char[count + 1];
			strncpy (line, st, count);
			wchar_t *l = new wchar_t[pos];
			line[pos] = 0;
			l[pos] = 0;
			mbstowcs (l, line, count);
			all_lines.push_back (l);
			st += count + 1;
			count = 0;
			continue;
		}
		count++;
	}

	delete []data;

	size_t size = all_lines.size ();

	wchar_t **all_text = new wchar_t *[size];

	for (size_t i = 0; i < size; i++) {
		all_text[i] = all_lines[i];
	}

	return all_text;
}

void Language::set_language (enum LANGUAGE type)
{
	if (type < 0 || type >= LANG_N) {
		std::cerr << "Not found language res" << std::endl;
		return;
	}

	cur_lang = langs[type];
}

wchar_t *Language::operator[] (enum TEXT offset)
{
	if (offset < 0 || offset >= TEXT_N) return nullptr;

	return cur_lang[offset];
}
