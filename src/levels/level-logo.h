#pragma once
#include <core/ilevel.h>
#include <core/graphics.h>
#include <core/texter.h>
#include <objects/sprite_int.h>
#include <core/sound.h>

class LevelLogo: public ILevel {
	public:
		void clear_screen (Graphics *) override;
		void load () override;
		void unload () override;
		void handle_button (enum EVENT_TYPE type, int x, int y) override;
		void render () override;

		SpriteInt *logo;
		Sound *sound_logo;
};
