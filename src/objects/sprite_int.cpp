#include <objects/sprite_int.h>
#include <core/globals.h>
#include <sys/time.h>
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

SpriteInt::SpriteInt ()
{
	if (global_width >= global_height)
		this->ortho = glm::ortho (-(global_width / 2.f), global_width / 2.f, global_height / 2.f, -(global_height / 2.f), 0.0f, 100.f);
	else
		this->ortho = glm::ortho (-1.0f, 1.0f, 1.0f * global_aspect, -1.0f * global_aspect, 0.0f, 100.f);

	this->cur_tex = 0;
	this->play = -1;
	this->is_display = 1;
	this->w = this->h = 0.0f;
	this->fact_width = 0;
	this->fact_height = 0;
}

void SpriteInt::set_pos (float x, float y, float z)
{
	this->pos = glm::translate (glm::mat4 (1.0f), glm::vec3 (x, y, z));
	this->x = x;
	this->y = y;
	this->z = z;
}

#include <stdio.h>
void SpriteInt::set_width (float width)
{
	set_size (w, h, 0);
}

void SpriteInt::set_height (float height)
{
	set_size (w, h, 0);
}

void SpriteInt::set_length (float length)
{
}

void SpriteInt::rotate (int offset, float degree)
{
}

void SpriteInt::set_size (float width, float height, float length)
{
	this->w = width;
        this->h = height;

	float w = width / 2.0f;
	float h = height / 2.0f;

        link->v[0] = -w;
        link->v[1] = -h;
        link->v[2] = 0;
        link->v[3] = -w;
        link->v[4] = h;
        link->v[5] = 0;
        link->v[6] = w;
        link->v[7] = -h;
        link->v[8] = 0;
        link->v[9] = w;
        link->v[10] = -h;
        link->v[11] = 0;
        link->v[12] = w;
        link->v[13] = h;
        link->v[14] = 0;
        link->v[15] = -w;
        link->v[16] = h;
        link->v[17] = 0;
}

void SpriteInt::calc_size (int _pow, int account_width_screen)
{
	int gheight = 270;
	int pow = 2;
	int standard = 1080;

	int gh = global_height;

	dec = 0;

	while (gheight < standard) {
		gheight *= 2;
		pow++;
	}
	pow += _pow;
	if (account_width_screen) {
		while (link->width * pow > global_width) {
			pow--;
			dec--;
		}
	}

	fact_width = link->width * pow;
	fact_height = link->height * pow;
	power = pow;

	set_size (fact_width, fact_height, 0);
}

void SpriteInt::mirror_right ()
{
        float ww = w / 2.0f;
        float hh = h / 2.0f;

        link->v [0] = -ww;
        link->v [1] = -hh;
        link->v [2] = 0;
        link->v [3] = -ww;
        link->v [4] = hh;
        link->v [5] = 0;
        link->v [6] = ww;
        link->v [7] = -hh;
        link->v [8] = 0;
        link->v [9] = ww;
        link->v [10] = -hh;
        link->v [11] = 0;
        link->v [12] = ww;
        link->v [13] = hh;
        link->v [14] = 0;
        link->v [15] = -ww;
        link->v [16] = hh;
        link->v [17] = 0;
}

void SpriteInt::mirror_left ()
{
        float ww = w / 2.0f;
        float hh = h / 2.0f;

        link->v[0] = ww;
        link->v[1] = -hh;
        link->v[2] = 0;
        link->v[3] = ww;
        link->v[4] = hh;
        link->v[5] = 0;
        link->v[6] = -ww;
        link->v[7] = -hh;
        link->v[8] = 0;
        link->v[9] = -ww;
        link->v[10] = -hh;
        link->v[11] = 0;
        link->v[12] = -ww;
        link->v[13] = hh;
        link->v[14] = 0;
        link->v[15] = ww;
        link->v[16] = hh;
        link->v[17] = 0;
}

void SpriteInt::set_animate_range (int index, int start, int end)
{
	int l = end - start;
	int total = l;
	{ end++; total++; }

	animations[index] = new uint32_t[total];
	indexes[index] = new uint32_t[total];
	int in = 0;
	for (uint32_t i = start; i < end; i++) {
		animations[index][in] = this->link->tex[i];
		indexes[index][in] = i;
		in++;
	}
	
	frames[index] = total;
}

void SpriteInt::set_speed (int offset, int speeds)
{
	speed[offset] = speeds;
}

void SpriteInt::play_once (int offset, int what)
{
	once[offset] = what;
}

static timeval tv;
static uint64_t cur_time;
static bool first_frame = true;

void SpriteInt::play_animate ()
{
	if (play >= 0) {
		gettimeofday (&tv, nullptr);
		uint64_t diff = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
		if (first_frame) {
			cur_time = diff;
			first_frame = false;
			tt[play] = 0L;
		}

		uint32_t prev = this->cur_tex;
		tt[play] += diff - cur_time;
		int32_t tex_index = tt[play] / speed[play] % frames[play];
		cur_tex = animations[play][tex_index];
		cur_tex_index = indexes[play][tex_index];
		if (cur_tex != animations[play][0] && once[play] == 1) once[play] = 2;
		if (cur_tex == animations[play][0] && once[play] == 2) {
			cur_tex = prev;
			once[play] = 1;
			is_stop = true;
			tt[play] = 0L;
			play = -1;
			first_frame = true;
		}
		cur_time = diff;
	}
}


void SpriteInt::render ()
{
	play_animate ();

	shader->render (this);
}

void SpriteInt::set_texture_index (int index) {
	cur_tex = link->tex[index];
	cur_tex_index = index;
}
