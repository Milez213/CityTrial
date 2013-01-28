
#ifndef ROCKET_H
#define ROCKET_H


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


class Rocket : Drawable {
    static const float ROCKET_SPEED = 20.0f;

    static const float EXP_LEN = 3.0f;

    enum Stage {ROCKET_TRAIL, ROCKET_EXPL, ROCKET_DONE};

public:
    vec3 vel;
    vec3 pos;

    Rocket(vec3 p, vec3 v, Mesh* rmesh, Shader* inmeshShader);

    ~Rocket();

    vec3 getPos();

    void render();

    void update(float t, float dt);

    void boom();

    bool dead();

    bool flying();

private:
    TFBParticles *flame;
    Sparks *explosion;
    Mesh *mesh;
    Shader *meshShader;

    int id;

    Stage stage;

    float exp_age;

};





#endif

