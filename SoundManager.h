#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

class GameSound {

public:

    virtual ~GameSound() {
    };

    // Plays the sound. Doesn't block
    // sound = SoundManager->getSample("woof.ogg");
    virtual void play() = 0; 

    virtual void play(bool looped) = 0; 

    virtual void pause() = 0;
    virtual void resume() = 0;
    
    // TODO - Callbacks when done playing?

    // TODO - position for 3d? inherent from GameObject?

    virtual void destroy() {
        fprintf(stderr, "Warning: '%s' shouldn't be called\n",
                    __func__);
    };

protected:
    int m_loops;

};


class SoundManager {

public:

    virtual ~SoundManager() {
    };

    // loads the sound. may cache it.
    virtual GameSound* getSample(char* filename) = 0;

    // For music. could use getSample but this is better
    virtual GameSound* getMusic(char* filename) = 0;

    virtual void destroy() {
        fprintf(stderr, "Warning: '%s' shouldn't be called\n",
                    __func__);
    };
};


#endif
