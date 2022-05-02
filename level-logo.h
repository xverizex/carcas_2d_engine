#pragma once
#include "ilevel.h"
#include "sprite.h"
#include "graphics.h"

class LevelLogo: public ILevel {
	public:
		void clear_screen (Graphics *) override;
		void load () override;
		void handle_button (enum EVENT_TYPE type, int x, int y) override;
		void render () override;

		Sprite *logo;

};
