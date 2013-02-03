#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

class GameSound {

public:

    virtual ~GameSound();

    // Plays the sound.
    // sound = ModelManager->getSound("woof.ogg");
    // Sound sound("woof");
    virtual void play() = 0; 

    // TODO - SDL_Mix can specify how many times to loop but not irrklang
    virtual void makeLooped() = 0;
    virtual bool isLooped() = 0;

    // TODO - position? inherent from GameObject?
    
};


class SoundManager {

public:

    SoundManager();

    virtual ~SoundManager();

    // loads the sound. may cache it.
    virtual GameSound* getSample(char* filename) = 0;

    // For music.
    virtual GameSound* getMusic(char* filename) = 0;
};


#endif
