#include "sound_android.h"
#ifdef __ANDROID__
#include <android/log.h>
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

Sound_android::Sound_android ()
{
    size_b = 0;
}

#define MAX_SL_AUDIO_FRAMES              8096 * 4
#define NUM_ANDROID_BUFFERS                     2

void Sound_android::init ()
{
    SLresult result;

    result = slCreateEngine (&engineObject, 0, NULL, 0, NULL, NULL);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }

    result = (*engineObject)->Realize (engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }

    result = (*engineObject)->GetInterface (engineObject, SL_IID_ENGINE, &eng);

}

void callback_player (SLPlayItf caller, void *ctx, uint32_t event)
{
    Sound *snd = (Sound *) ctx;
    snd->state = 1;
}

void Sound_android::set (char *file)
{
    size_t pos = 0L;
    uint8_t *data = (uint8_t *) sound_get_data (file, &pos);


    (*eng)->CreateOutputMix (eng, &mix, 0, 0, 0);
    (*mix)->Realize (mix, SL_BOOLEAN_FALSE);

    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, nullptr};

    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource ds = {&que, &pcm};

    SLAndroidSimpleBufferQueueItf pcmQueItf = nullptr;
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE, SL_IID_PLAY};
    const SLboolean req[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    (*eng)->CreateAudioPlayer (eng, &player, &ds, &audioSink, sizeof (ids) / sizeof (SLInterfaceID), ids, req);
    (*player)->Realize (player, SL_BOOLEAN_FALSE);

    //(*player)->GetInterface (player, SL_IID_PLAY, &playerItf);
    (*player)->GetInterface (player, SL_IID_PLAY, &playerItf);
    (*player)->GetInterface (player, SL_IID_BUFFERQUEUE, &pcmQueItf);
    (*playerItf)->SetCallbackEventsMask (playerItf, SL_PLAYEVENT_HEADATEND);
    (*playerItf)->RegisterCallback (playerItf, callback_player, this);

    (*pcmQueItf)->Enqueue (pcmQueItf, data, pos);


    state = 0;
}

void Sound_android::play ()
{
    (*playerItf)->SetPlayState (playerItf, SL_PLAYSTATE_PLAYING);
    //(*pcmQueItf)->Enqueue (pcmQueItf, "", 1);
}

void Sound_android::stop ()
{
    (*playerItf)->SetPlayState (playerItf, SL_PLAYSTATE_STOPPED);
}

bool Sound_android::is_no_play ()
{
    if (state == 1) return true;

    return false;
}