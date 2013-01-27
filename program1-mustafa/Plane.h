
#ifndef PLANE_H
#define PLANE_H


#include <iostream>
#include <GL/glut.h>

#include "drawable.h"

#include "mesh.h"
#include "tfbparticles.h"
#include "sparks.h"
#include "Shader.h"
#include "defines.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc



// global variables

using glm::translate;
using glm::scale;
using glm::rotate;


class Plane : Drawable {
    static const float ROCKET_SPEED = 10.0f;

    static const float EXP_LEN = 3.0f;

    enum Stage {FLYING, TRAIL, EXPL, DONE};

public:
    vec3 vel;
    vec3 pos;

    Plane(vec3 p, vec3 v, Mesh* rmesh, Shader* inmeshShader);

    ~Plane();

    vec3 getPos();

    void render();

    void update(float t, float dt);

    void boom();

    bool dead();

    bool flying();

    void down();

private:
    TFBParticles *smoke;
    Sparks *explosion;
    Mesh *mesh;
    Shader *meshShader;

    mat4 s;
    mat4 r;

    mat4 rdown;

    int id;

    Stage stage;

    float exp_age;

};





#endif

