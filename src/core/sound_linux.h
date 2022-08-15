//
// Created by cf on 5/14/22.
//

#ifndef ONE_IN_TAIGA_SOUND_LINUX_H
#define ONE_IN_TAIGA_SOUND_LINUX_H

#include <stdint.h>
#include "sound.h"

#include <AL/al.h>
#include <AL/alc.h>

class Sound_linux: public Sound {
	public:
    Sound_linux ();
    void init () override;
    void set (char *file, int format, int freq) override;
    void play () override;
    void stop () override;
    bool is_no_play () override;

    uint32_t buffer[1];
    uint32_t size_b;
};


#endif //ONE_IN_TAIGA_SOUND_LINUX_H
