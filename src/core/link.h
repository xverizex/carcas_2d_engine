#pragma once
#include <stdint.h>

#define VERTEX_COORD              0
#define TEXTURE_COORD             1
class Link {
	public:
		uint32_t *tex;
		uint32_t size_tex;
		int width;
		int height;
		float v[18];
		float t[12];
};
