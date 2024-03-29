#pragma once
#include <core/link.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <core/shader.h>


class Object {
	public:
		Shader *shader;
		Link *link;
		float width;
		float height;
		float length;
		uint64_t fact_width;
		uint64_t fact_height;
		float x;
		float y;
		float z;
		float w;
		float h;
		int is_display;

		virtual void set_pos (float x, float y, float z) = 0;
		virtual void set_width (float width) = 0;
		virtual void set_height (float height) = 0;
		virtual void set_length (float length) = 0;
		virtual void set_size (float width, float height, float length) = 0;
		virtual void rotate (int offset, float degree) = 0;
		virtual void calc_size (int pow, int account_width_screen) = 0;
		virtual void mirror_right () = 0;
		virtual void mirror_left () = 0;
		virtual void set_animate_range (int index, int start, int end) = 0;
		virtual void play_once (int offset, int what) = 0;
		virtual void set_speed (int offset, int speeds) = 0;
		virtual void play_animate () = 0;
		virtual void render () = 0;
};
