#pragma once
#include <core/ilevel.h>
#include <objects/sprite.h>
#include <core/graphics.h>
#include <core/texter.h>

class LevelLogo: public ILevel {
	public:
		void clear_screen (Graphics *) override;
		void load () override;
		void handle_button (enum EVENT_TYPE type, int x, int y) override;
		void render () override;

		Sprite *logo;
		Sprite *text;
		Texter *texter;
};
