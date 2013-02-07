#ifndef Dummy_SOUND_MANAGER_H
#define Dummy_SOUND_MANAGER_H

#include <stdio.h>

#include "SoundManager.h"


class DummyGameSoundSample : public GameSound {

public:

    DummyGameSoundSample(const char* filename) { }

    ~DummyGameSoundSample() { }

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    void play() { } 

    void play(bool looped) { }

    void pause() { }

    void resume() { }

};



class DummySoundManager : public SoundManager {

public:

    DummySoundManager() {
        printf("Using Dummy sound\n");
    }

    ~DummySoundManager() {
    }

    virtual GameSound* getMusic(const char* filename) {
        return getSample(filename);
    }

    virtual GameSound* getSample(const char* filename) {
        return new DummyGameSoundSample(filename);
    }
};


#endif
