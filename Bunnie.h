
#ifndef BUNNIE_H
#define BUNNIE_H


#include <iostream>

#include "include_glu.h"


#include "Drawable.h"
#include "Mesh.h"
#include "FlatShader.h"
#include "defines.h"

#include "include_glm.h"

// global variables

using glm::translate;
using glm::scale;
using glm::rotate;


class Bunnie : Drawable {
    static const float BUNNIE_SPEED = 0.5f;

    enum Stage {MOVING, STILL, DONE};

public:
    vec3 vel;
    vec3 pos;
    Stage stage;

    Bunnie(vec3 p, vec3 v, Mesh* rmesh, FlatShader* inmeshShader);

    ~Bunnie();

    vec3 getPos();

    void draw();

    bool collide(vec3 pos, float r);

    void update(double dt);
    
    void stop();
    
    void done() {
        stage = DONE;
    }
    

private:
    Mesh *mesh;
    FlatShader *meshShader;

    mat4 s;
    mat4 r;

    int id;


};





#endif

