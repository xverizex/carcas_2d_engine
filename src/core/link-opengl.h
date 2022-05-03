#pragma once
#include <stdint.h>
#include "link.h"

class LinkOpenGL: public Link {
	public:
		~LinkOpenGL ();
		uint32_t *tex;
		uint32_t size_tex;
		uint32_t cur_tex = { 0 };
		int width;
		int height;
		float v[18];
		float t[12];
};
