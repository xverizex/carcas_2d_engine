#pragma once
#include <core/object.h>

#define SPRITE_MAX_FRAMES             30

class Sprite: public Object {
	public:
		Sprite ();
		void set_pos (float x, float y, float z) override;
		void set_width (float width) override;
		void set_height (float height) override;
		void set_length (float length) override;
		void set_size (float width, float height, float length) override;
		void rotate (int offset, float degree) override;
		void calc_size () override;
		void set_animate_range (int index, int start, int end) override;
		void play_once (int offset, int what) override;
		void set_speed (int offset, int speeds) override;
		void play_animate () override;
		void mirror_right () override;
		void mirror_left () override;
		void render () override;

        glm::vec4 color;
		glm::mat4x4 pos;
		glm::mat4x4 ortho;
		uint32_t *animations[SPRITE_MAX_FRAMES];
		uint32_t frames[SPRITE_MAX_FRAMES];
		uint32_t speed[SPRITE_MAX_FRAMES];
		uint32_t once[SPRITE_MAX_FRAMES];
		int play;
		uint32_t cur_tex;
};
