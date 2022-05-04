#include <levels/level-logo.h>
#include <core/languages.h>
#include <text.h>
#include <core/globals.h>

extern Language global_language;

void LevelLogo::clear_screen (Graphics *graphics)
{
	graphics->clear (0x4c, 0x4c, 0x4c, 0xff);
}

void LevelLogo::load ()
{
	global_language.set_language (LANG_RUS);

	logo = new Sprite ();
	logo->shader = global_shader[SHADER_MAIN];
	logo->link = downloader_load (LINK_LOGO);
	logo->set_pos (0.0f, 0.0f, 0.0f);
	logo->set_width (1.0f);

	texter = new Texter ();

	text = new Sprite ();
	text->shader = global_shader[SHADER_MAIN];
	text->link = texter->generate_link (global_language[TEXT_HELLO_WORLD], 18, 18 * 20);
	text->set_pos (0.0f, 0.0f, 0.0f);
	text->set_width (2.0f);
}

void LevelLogo::handle_button (enum EVENT_TYPE type, int x, int y)
{
}

void LevelLogo::render ()
{
	logo->render ();
	text->render ();
}
