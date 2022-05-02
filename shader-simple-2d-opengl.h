#pragma once
#include "shader.h"
#include <stdint.h>

class ShaderSimple2DOpenGL: public Shader {
	public:
		ShaderSimple2DOpenGL ();
		void init (uint32_t program) override;
		void render (void *data) override;

		uint32_t program;
		uint32_t uniform_ortho;
		uint32_t uniform_position;
		uint32_t uniform_cur_texture;
};
