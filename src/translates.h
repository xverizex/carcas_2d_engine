#pragma once

enum LANGUAGE {
	LANG_ENG,
	LANG_RUS,
	LANG_N
};

#define LANG_TO_STRING_RES(name) name##_RES

#ifdef __ANDROID__
#define LANG_ENG_RES                          "lang/eng.txt"
#define LANG_RUS_RES                          "lang/rus.txt"
#else
#define LANG_ENG_RES                          "assets/lang/eng.txt"
#define LANG_RUS_RES                          "assets/lang/rus.txt"
#endif

inline char **init_list_lang ()
{
	char **file = new char *[LANG_N];
	
	file[LANG_ENG] = LANG_TO_STRING_RES (LANG_ENG);
	file[LANG_RUS] = LANG_TO_STRING_RES (LANG_RUS);

	return file;
}
