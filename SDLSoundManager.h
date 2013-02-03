#ifndef SDL_SOUND_MANAGER_H
#define SDL_SOUND_MANAGER_H

#include <stdio.h>

#include "SoundManager.h"

#include "SDL_mixer.h"

class SDLGameSoundSample : GameSound {

public:

    SDLGameSoundSample() {

    }

    virtual ~SDLGameSoundSample() {
        
    }

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    virtual void play() = 0; 

    // TODO - SDL_Mix can specify how many times to loop but not irrklang
    virtual void makeLooped() = 0;
    virtual bool isLooped() = 0;

    // TODO - position? inherent from GameObject?

private:
    Mix_Music *m_music; // FIXME - change
    Mix_Chunk *m_sound;

    int m_channel;
    
};


class SDLSoundManager {

public:

    SDLSoundManager() {
        int sample_rate = 22050;
        Uint16 audio_format = AUDIO_S16;
        int audio_channels = 2;

        // smaller for more responsiveness, but needs more CPU
        int audio_buffer_size = 1024;

        Mix_Init(MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MP3);

        if (Mix_OpenAudio(sample_rate, audio_format, audio_channels,
                          audio_buffer_size)) {
            fprintf(stderr, "SDL_Mix: Couldn't open audio device: %s\n",
                    Mix_GetError());
            return;
        }

    }

    virtual ~SDLSoundManager() {
        Mix_CloseAudio();
    }

    // loads the sound. may cache it.
    virtual GameSound* getMusic(char* filename) {
        SLDSound *sound = new SDLSound();

    }

    // loads the sound. may cache it.
    virtual GameSound* getSample(char* filename) {
        SLDSound *sound = new SDLSound();

    }
};


#endif
