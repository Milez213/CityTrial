#ifndef PLANE_MANAGER_H
#define PLANE_MANAGER_H


#include "Plane.h"

// for collision detection
#include "RocketManager.h"

#include <list>


class Planes {
    static const int MAX_DIST = 50.0f;

public:
    Planes(Shader* shader, Rockets* rocks);

    void update(float t, float dt);

    void boomAll();

    // == render rocket
    void render();

    void add(vec3 p, vec3 v);


private:
    float randFloat();

    list<Plane> m_planes;

    Mesh *mesh;
    Shader *meshShader;

    float creation_delay;
    float last_created;

    Rockets *rockets;

};


#endif

