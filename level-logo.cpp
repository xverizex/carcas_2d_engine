#include "level-logo.h"

void LevelLogo::clear_screen (Graphics *graphics)
{
	graphics->clear (0x4c, 0x4c, 0x4c, 0xff);
}

void LevelLogo::load ()
{
	logo = new Sprite ();
	logo->shader = global_shader[SHADER_MAIN];
	logo->link = downloader_load (LINK_LOGO);
	logo->set_pos (0.0f, 0.0f, 0.0f);
	logo->set_width (1.0f);
}

void LevelLogo::handle_button (enum EVENT_TYPE type, int x, int y)
{
}

void LevelLogo::render ()
{
	logo->render ();
}
