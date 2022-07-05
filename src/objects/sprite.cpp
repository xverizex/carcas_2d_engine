#include <objects/sprite.h>
#include <core/globals.h>
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

Sprite::Sprite ()
{
	if (global_width >= global_height)
		this->ortho = glm::ortho (-1.0f * global_aspect, 1.0f * global_aspect, 1.0f, -1.0f, 0.0f, 100.f);
	else
		this->ortho = glm::ortho (-1.0f, 1.0f, 1.0f * global_aspect, -1.0f * global_aspect, 0.0f, 100.f);

	this->cur_tex = 0;
	this->play = -1;
	this->is_display = 1;
	this->w = this->h = 0.0f;
}

void Sprite::set_pos (float x, float y, float z)
{
	this->pos = glm::translate (glm::mat4 (1.0f), glm::vec3 (x, y, z));
	this->x = x;
	this->y = y;
	this->z = z;
}

#include <stdio.h>
void Sprite::set_width (float width)
{
	float aspect = 0.0f;

	if (link->width == link->height) {
        if (global_width < global_height) {
            w = width;
            h = w;
        } else {
            w = width;
            h = w;
        }

	} else if (link->width > link->height) {
        if (global_width < global_height) {
            aspect = (float) link->width / (float) link->height;
            w = width;
            h = w / aspect;
        } else {
            aspect = (float) link->width / (float) link->height;
            w = width;
            h = w / aspect;
        }

	} else {
        if (global_width < global_height) {
            aspect = (float) link->height / (float) link->width;
            w = width;
            h = w * aspect;
        } else {
            aspect = (float) link->height / (float) link->width;
            w = width;
            h = w * aspect;
        }

	}

	set_size (w, h, 0);
}

void Sprite::set_height (float height)
{
	float aspect = 0.0f;

    if (link->width == link->height) {
        if (global_width < global_height) {
            h = height;
            w = h;
        } else {
	    aspect = (float) global_width / (float) global_height;
            h = height;
            w = h / aspect / aspect;
        }

    } else if (link->width > link->height) {
        if (global_width < global_height) {
            aspect = (float) link->width / (float) link->height;
            h = height;
            w = h * aspect;
        } else {
            aspect = (float) link->width / (float) link->height;
            h = height;
            w = h * aspect;
        }

    } else {
        if (global_width < global_height) {
            aspect = (float) link->height / (float) link->width;
            h = height;
            w = h / aspect;
        } else {
            aspect = (float) link->height / (float) link->width;
            h = height;
            w = h / aspect;
        }

    }

	set_size (w, h, 0);
}

void Sprite::set_length (float length)
{
}

void Sprite::rotate (int offset, float degree)
{
}

void Sprite::set_size (float width, float height, float length)
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

void Sprite::calc_size ()
{
}

void Sprite::mirror_right ()
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

void Sprite::mirror_left ()
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

void Sprite::set_animate_range (int index, int start, int end)
{
	int total = end - start;
	animations[index] = new uint32_t[total];
	int in = 0;
	for (int i = start; i < end; i++) {
		animations[index][in++] = this->link->tex[i];
	}
	
	frames[index] = total;
}

void Sprite::set_speed (int offset, int speeds)
{
	speed[offset] = speeds;
}

void Sprite::play_once (int offset, int what)
{
	once[offset] = what;
}

void Sprite::play_animate ()
{
	if (play >= 0) {
		uint32_t prev = this->cur_tex;
		cur_tex = SDL_GetTicks () / speed[play] % frames[play];
		if (cur_tex != 0 && once[play] == 1) once[play] = 2;
		if (cur_tex == 0 && once[play] == 2) {
			cur_tex = prev;
			play = -1;
		}
	}
}


void Sprite::render ()
{
	play_animate ();

	shader->render (this);
}
