#ifndef SDL_SOUND_MANAGER_H
#define SDL_SOUND_MANAGER_H

#include <stdio.h>

#include "SoundManager.h"

#include "SDL_mixer.h"

class SDLGameSoundSample : public GameSound {

public:

    SDLGameSoundSample(const char* filename) {
        m_channel = -1;
        m_loops = 0; // play once
        m_sound = Mix_LoadWAV(filename);
        if (NULL == m_sound) {
            fprintf(stderr, "*** Error loading sound \"%s\": %s\n",
                    filename,
                    Mix_GetError());
        }
    }

    ~SDLGameSoundSample() {
        Mix_FreeChunk(m_sound);
        Mix_HaltChannel(m_channel);
    }

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    void play() {
        m_channel = Mix_PlayChannel(-1, m_sound, m_loops);
    }

    void play(bool looped) {
        m_channel = Mix_PlayChannel(-1, m_sound, looped? -1 : 0);
    }

    void fadeIn(int ms, int loops) {
        m_channel = Mix_FadeInChannel(-1, m_sound, loops, ms);
    }

    void fadeOut(int ms) {
        m_channel = Mix_FadeOutChannel(-1, ms);
    }

    void pause() {
        Mix_Pause(m_channel);
    }

    void resume() {
        Mix_Resume(m_channel);
    }

    void setVolume(int v){
       Mix_Volume(m_channel,v);
    }

    // TODO - SDL_Mix can specify how many times to loop but not irrklang

private:
    // a sound is pointed to by m_sound
    // a sound plays on one of the channels, which SDL_Mix can choose
    Mix_Chunk *m_sound;
    int m_channel;
};



class SDLGameSoundMusic : public GameSound {

public:

    SDLGameSoundMusic(const char* filename) {
        m_loops = 0;
        m_music = Mix_LoadMUS(filename);
        if (NULL == m_music) {
            fprintf(stderr, "*** Error loading sound \"%s\": %s\n",
                    filename,
                    Mix_GetError());
        }

    }

    ~SDLGameSoundMusic() {
        Mix_HaltMusic();
        Mix_FreeMusic(m_music);
    }

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    virtual void play() {
        Mix_PlayMusic(m_music, m_loops);
    }

    virtual void play(bool looped) {
        Mix_PlayMusic(m_music, looped? -1 : 0);
    }

    virtual void pause() {
        Mix_PauseMusic();
    }

    virtual void resume() {
        Mix_ResumeMusic();
    }

    // TODO - SDL_Mix can specify how many times to loop but not irrklang
    virtual void setLooped(bool looped) {
        m_loops = looped? -1 : 0;
    }
    virtual bool isLooped() {
        return m_loops == -1;
    }

    virtual bool setVolume(int v){
       Mix_VolumeMusic(v);
    }

private:
    Mix_Music *m_music;

};



class SDLSoundManager : public SoundManager {

public:

    SDLSoundManager() {
        printf("Initializing SDL_Mix sound\n");
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
        printf("Initializing SDL_Mix sound successful\n");
    }

    ~SDLSoundManager() {
        Mix_CloseAudio();
    }

    // loads the sound. may cache it.
    virtual GameSound* getMusic(const char* filename) {
        // TODO - cache?
        GameSound *sound = new SDLGameSoundMusic(filename);
        return sound;
    }

    // loads the sound. may cache it.
    virtual GameSound* getSample(const char* filename) {
        // TODO - cache?
        SDLGameSoundSample *sound = new SDLGameSoundSample(filename);

        return sound;
    }
};


#endif
