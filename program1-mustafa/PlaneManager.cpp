
#include "PlaneManager.h"

#include <math.h>



Planes::Planes(Shader* shader, Rockets* inrockets) {
    meshShader = shader;
    mesh = new Mesh("cessna500.m");

    // one plane every 4 seconds
    creation_delay = 4.0f;
    last_created = 0;

    rockets = inrockets;
}

void Planes::update(float t, float dt) {

    if (t * MS_2_S > (last_created + creation_delay)) {
        
        add(vec3(-10 + 20 * randFloat(), 5 + 5 * randFloat(), -30), vec3(0, 0, 1));
        last_created = t * MS_2_S;
        cout << "new plane\n";
    }


    list<Plane>::iterator it;


    for (it=m_planes.begin(); it != m_planes.end();) {

        it->update(t, dt);

        // if ((glm::length(it->getPos()) > MAX_DIST) && it->flying()) {
        if (it->flying()) {
            // (glm::length(it->getPos()) > MAX_DIST) || it->pos.y < -1)
            //

            if (rockets->collide(it->pos, 2)) {
                it->down();
            }
            
            it++;
        } else if (it->pos.y < -1) {

            it->boom();
            it++;
        } else if (it->dead()) {
            // it->~Rocket();
            m_planes.erase(it++);
        }
        else {
            it++;
        }
    }
}

float Planes::randFloat() {
    return ((float) rand() / RAND_MAX);
}


void Planes::render() {
    list<Plane>::iterator it;
    for (it=m_planes.begin(); it != m_planes.end(); it++) {
        it->render();
    }
}

void Planes::boomAll() {
    list<Plane>::iterator it;
    for (it=m_planes.begin(); it != m_planes.end(); it++) {
        it->boom();
    }
}


void Planes::add(vec3 p, vec3 v) {
    Plane *r = new Plane(p, v, mesh, meshShader);
    // r->pos += v;

    m_planes.push_back(*r);
    cout << "num: " << m_planes.size()-1 << endl;
}

