
#ifndef GAME_KART_H
#define GAME_KART_H


#include <iostream>
#include <vector>

#include <stdlib.h>
#include <stdarg.h>

#include "include_glu.h"


;
#include "GameDrawableObject.h"
#include "GameObject.h"


#include "FlatShader.h"
#include "defines.h"

#include "include_glm.h"

// global variables
using std::vector;
using glm::translate;
using glm::scale;
using glm::rotate;


class GameKartObject : GameObject {
    static const float KART_SPEED = 0.5f;

    enum Stage {MOVING, STILL, DONE};



public:

        GameDrawableObject *wheel[4];
    GameDrawableObject *chassis;

    GameKartObject(glm::vec3 p, FlatShader* inmeshShader);

    ~GameKartObject();

    vec3 getPos();

    void draw();

    bool collide(vec3 pos, float r);

    void update(double dt);
    
    void stop();
    
    void done() {
     
    }
    

private:

    FlatShader *meshShader;
 


    mat4 s;
    mat4 r;
    Stage stage;
    int id;


};





#endif
