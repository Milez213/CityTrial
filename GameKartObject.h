
#ifndef GAME_KART_H
#define GAME_KART_H


#include <iostream>

#include "include_glu.h"


#include "Drawable.h"
#include "GameDrawableObject.h"
#include "GameObject.h"

#include "Mesh.h"
#include "FlatShader.h"
#include "defines.h"

#include "include_glm.h"

// global variables

using glm::translate;
using glm::scale;
using glm::rotate;


class GameKartObject : GameObject {
    static const float KART_SPEED = 0.5f;

    enum Stage {MOVING, STILL, DONE};

public:


    GameKartObject(glm::vec3 p, glm::vec3 v,Mesh *rmesh, FlatShader* inmeshShader);

    ~GameKartObject();

    vec3 getPos();

    void draw();

    bool collide(vec3 pos, float r);

    void update(double dt);
    
    void stop();
    
    void done() {
     
    }
    

private:
    Mesh *mesh;
    FlatShader *meshShader;
 
    GameDrawableObject *wheel[4];
    GameDrawableObject *chassis;

    mat4 s;
    mat4 r;
    Stage stage;
    int id;


};





#endif
