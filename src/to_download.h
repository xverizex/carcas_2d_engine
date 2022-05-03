#pragma once

#define TO_STRING_FILENAME(name) name##_STRING


enum TO_DOWNLOADS {
	LINK_LOGO,
	LINKS_N
};

#ifdef __ANDROID__
#define LINK_LOGO_STRING                  "logo.res"
#else
#define LINK_LOGO_STRING                  "assets/logo.res"
#endif
