
#include "Bunnies.h"

#include <math.h>



Bunnies::Bunnies(Shader* shader) {
    meshShader = shader;
    mesh = new Mesh("bunny500.m");

    // one plane every 4 seconds
    creation_delay = 4.0f;
    last_created = 0;
}

void Bunnies::update(float t, float dt, vec3 player_pos, int *score) {

    float angle = (randFloat() * 2 * PI);

    if (t * MS_2_S > (last_created + creation_delay)) {
        
        add(vec3(-10 + 20 * randFloat(), 0.5 , -10 + 10 * randFloat()),
            vec3(sin(angle), 0, cos(angle)));
        last_created = t * MS_2_S;
        cout << "new plane\n";
    }


    list<Bunnie>::iterator it;
    list<Bunnie>::iterator it2;


    for (it=m_bunnies.begin(); it != m_bunnies.end(); it++) {
        it->update(t, dt);
    }

    // check for collisions
    for (it=m_bunnies.begin(); it != m_bunnies.end(); it++) {
        for (it2 = it; it2 != m_bunnies.end();) {
            it2++;
            if (it2 != m_bunnies.end())
            if (it->collide(it2->pos, 1)) {
                it->stop();
            }
        }

        if (it->collide(player_pos, 2)) {
            it->done();
            *score = (*score) + 1;
        }
    }


}

float Bunnies::randFloat() {
    return ((float) rand() / RAND_MAX);
}


void Bunnies::render() {
    list<Bunnie>::iterator it;
    for (it=m_bunnies.begin(); it != m_bunnies.end(); it++) {
        it->render();
    }
}

void Bunnies::add(vec3 p, vec3 v) {
    Bunnie *r = new Bunnie(p, v, mesh, meshShader);
    // r->pos += v;

    m_bunnies.push_back(*r);
    cout << "num: " << m_bunnies.size()-1 << endl;
}

