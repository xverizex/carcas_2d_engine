#include <levels/level-logo.h>
#include <core/languages.h>
#include <text.h>
#include <core/globals.h>
#include <levels/levels.h>
#ifdef __ANDROID__
#include <core/sound_android.h>
#else
#include <core/sound_linux.h>
#endif
#include <sys/time.h>

extern Language global_language;

void LevelLogo::clear_screen (Graphics *graphics)
{
	graphics->clear (0x4c, 0x4c, 0x4c, 0xff);
}

void LevelLogo::load ()
{
	from = to = LEVEL_LOGO;
//	global_language.set_language (LANG_RUS);

	logo = new SpriteInt ();
	logo->shader = global_shader[SHADER_MAIN_INT];
	logo->link = downloader_load (LINK_LOGO);
	logo->set_pos (0.0f, 0.0f, 0.0f);
	logo->calc_size(3);
	logo->set_texture_index(0);

#ifdef __ANDROID__
	sound_logo = new Sound_android ();
	sound_logo->init ();
	sound_logo->set ("logo.wav", FORMAT_MONO, 44100);

#else
	sound_logo = new Sound_linux ();
	sound_logo->init ();
	sound_logo->set ("assets/logo.wav", FORMAT_MONO, 88200);
#endif

	sound_logo->play();
}

void LevelLogo::handle_button (enum EVENT_TYPE type, int x, int y)
{
}

void LevelLogo::unload () {
}

#include <stdint.h>

static uint64_t cur_time;
static uint64_t to_stop;

void LevelLogo::render ()
{
	struct timeval tv_stage_0;
	gettimeofday (&tv_stage_0, nullptr);
	uint64_t ms = (tv_stage_0.tv_sec * 1000) + (tv_stage_0.tv_usec / 1000);

	if (cur_time == 0L) {
		cur_time = ms;
	}

	uint64_t diff_time = ms - cur_time;

	to_stop += diff_time;

	if (to_stop > 9000) {
		to = LEVEL_MAP;
	}

	logo->render();

	cur_time = ms;
}
