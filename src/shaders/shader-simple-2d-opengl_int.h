#pragma once
#include <core/shader.h>
#include <stdint.h>

class ShaderSimple2DOpenGLInt: public Shader {
	public:
		ShaderSimple2DOpenGLInt ();
		void init (uint32_t program) override;
		void render (void *data) override;

		uint32_t program;
		uint32_t uniform_ortho;
		uint32_t uniform_position;
		uint32_t uniform_cur_texture;
};
