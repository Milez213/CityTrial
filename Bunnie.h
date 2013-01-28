
#ifndef BUNNIE_H
#define BUNNIE_H


#include <iostream>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "Drawable.h"
#include "Mesh.h"
#include "FlatShader.h"
#include "defines.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc



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

    void render();

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
