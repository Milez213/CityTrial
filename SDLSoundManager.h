#ifndef SDL_SOUND_MANAGER_H
#define SDL_SOUND_MANAGER_H

#include <stdio.h>

#include "SoundManager.h"

#include "SDL_mixer.h"

class SDLGameSoundSample : GameSound {

public:

    SDLGameSoundSample(char* filename) {
        m_channel = -1;
        m_sound = Mix_LoadWAV(filename);
        if (NULL == m_sound) {
            fprintf(stderr, "*** Error loading sound \"%s\": %\n",
                    filename,
                    Mix_GetError());
        }

    }

    virtual ~SDLGameSoundSample() {
        Mix_HaltChannel(m_channel);
    }

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    virtual void play() {
        m_channel = Mix_PlayChannel(-1, m_sound, m_loops);
    }

    virtual void play(bool looped) {
        m_channel = Mix_PlayChannel(-1, m_sound, -1);
    }

    virtual void pause() {
        Mix_Pause(m_channel);
    }

    virtual void resume() {
        Mix_resume(m_channel);
    }

    // TODO - SDL_Mix can specify how many times to loop but not irrklang
    virtual void setLooped(bool looped) {
        m_loops = looped? -1 : 0;
    }
    virtual bool isLooped() {
        return m_loops == -1;
    }

private:
    Mix_Music *m_music; // FIXME - change
    Mix_Chunk *m_sound;

    int m_channel;

    int m_loops = 0;
    
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
        // TODO - cache?
        SLDSound *sound = new SDLGameSoundMusic(filename);
        return sound;
    }

    // loads the sound. may cache it.
    virtual GameSound* getSample(char* filename) {
        // TODO - cache?
        SLDSound *sound = new SDLGameSoundSample(filename);

        return sound;
    }
};


#endif
