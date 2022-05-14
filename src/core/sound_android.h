//
// Created by cf on 5/14/22.
//

#ifndef ONE_IN_TAIGA_SOUND_ANDROID_H
#define ONE_IN_TAIGA_SOUND_ANDROID_H

#include "sound.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <stdint.h>

class Sound_android: public Sound {
public:
    Sound_android ();
    void init () override;
    void set (char *file) override;
    void play () override;
    void stop () override;
    bool is_no_play () override;

    uint32_t buffer[1];
    uint32_t size_b;

    SLObjectItf engineObject;
    SLEngineItf eng;
    SLObjectItf mix;
    SLObjectItf player;
    SLPlayItf playerItf;
    SLPrefetchStatusItf pre;
    SLAndroidSimpleBufferQueueItf pcmQueItf;
    uint32_t state;

    uint8_t *data;
};


#endif //ONE_IN_TAIGA_SOUND_ANDROID_H
