#pragma once

#include <wchar.h>
#include "translates.h"
#include "text.h"
#include <cstdint>

class Language {
	public:
		Language ();
		void set_language (enum LANGUAGE type);
		wchar_t *operator[] (enum TEXT offset);
	private:
		wchar_t **cur_lang;
		wchar_t **langs[LANG_N];
		int64_t text_count[LANG_N];
		
		wchar_t **read_and_convert_to_wchar (const char *file);
};
