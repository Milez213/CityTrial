#ifndef BUNNIES_H
#define BUNNIES_H


#include "Bunnie.h"

// for collision detection
#include "Bunnies.h"

#include <list>


class Bunnies {
    static const int MAX_DIST = 20.0f;

public:
    Bunnies(Shader* shader);

    void update(float t, float dt, vec3 player_pos, int *score);

    // == render rocket
    void render();

    void add(vec3 p, vec3 v);

    int get_num_bunnies() {
        return m_bunnies.size();
    }

private:
    float randFloat();

    list<Bunnie> m_bunnies;

    Mesh *mesh;
    Shader *meshShader;

    float creation_delay;
    float last_created;

};


#endif

