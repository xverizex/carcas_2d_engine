#pragma once
#include <stdint.h>
#ifdef __ANDROID__
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#else
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#endif

class Shader {
	public:
		virtual void init (uint32_t type) = 0;
		virtual void render (void *data) = 0;
};
