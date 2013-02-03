#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

class GameSound {

public:

    virtual ~GameSound();

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    virtual void play() = 0; 

    virtual void play(bool looped) = 0; 

    virtual void makeLooped() = 0;
    virtual bool isLooped() = 0;

    virtual void pause() = 0;
    virtual bool resume() = 0;

    // TODO - position for 3d? inherent from GameObject?
    
};


class SoundManager {

public:

    SoundManager();

    virtual ~SoundManager();

    // loads the sound. may cache it.
    virtual GameSound* getSample(char* filename) = 0;

    // For music. could use getSample but this is better
    virtual GameSound* getMusic(char* filename) = 0;
};


#endif
