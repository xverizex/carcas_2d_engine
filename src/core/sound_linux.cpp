#include "sound_linux.h"
#include <SDL2/SDL.h>

Sound_linux::Sound_linux ()
{
    size_b = 0;
}

static ALCdevice *device;
static ALCcontext *ctx;

void Sound_linux::init ()
{
	if (device == nullptr) {
    		device = alcOpenDevice (nullptr);
    		ctx = alcCreateContext (device, nullptr);
    		alcMakeContextCurrent (ctx);
	}
}



void Sound_linux::set (char *file, int format, int freq)
{
    uint32_t buf;
    alGenBuffers (1, &buf);
    switch (format) {
	    case FORMAT_MONO:
		    format = AL_FORMAT_MONO16;
		    break;
	    case FORMAT_STEREO:
		    format = AL_FORMAT_STEREO16;
		    break;
    }

    size_t size;
    void *data = sound_get_data (file, &size);
    alBufferData (buf, format, data, size, freq);

    int ret = alGetError ();
    printf ("0: %d\n", ret);

    uint32_t source;
    alGenSources (1, &source);
    alSourcei (source, AL_BUFFER, buf);

    buffer[0] = source;
}

void Sound_linux::play ()
{
    alSourcePlay (buffer[0]);
}

void Sound_linux::stop ()
{
    alSourceStop (buffer[0]);
}

bool Sound_linux::is_no_play ()
{
    int state;
    alGetSourcei (buffer[0], AL_SOURCE_STATE, &state);
    if (state == AL_STOPPED) return true;
    return false;
}
