#ifndef ROCKET_MANAGER_H
#define ROCKET_MANAGER_H

#include "Rocket.h"
#include <list>


class Rockets {
    static const int MAX_DIST = 50.0f;

public:
    Rockets(Shader* shader);

    void update(float t, float dt);

    void boomAll();

    // == render rocket
    void render();

    void add(vec3 p, vec3 v);
    bool collide(vec3 pos, float r);


private:
    list<Rocket> m_rockets;

    Mesh *rocketMesh;
    Shader *meshShader;

};


#endif

