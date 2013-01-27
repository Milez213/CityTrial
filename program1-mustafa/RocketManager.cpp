
#include "RocketManager.h"



Rockets::Rockets(Shader* shader) {
    meshShader = shader;
    rocketMesh = new Mesh("cube.m");
}



// in sphere of radius d and pos ?
bool Rockets::collide(vec3 pos, float r) {
    list<Rocket>::iterator it;

    for (it=m_rockets.begin(); it != m_rockets.end(); it++) {

        if ( ((it->pos.x-pos.x) * (it->pos.x-pos.x) +
              (it->pos.y-pos.y) * (it->pos.y-pos.y) +
              (it->pos.z-pos.z) * (it->pos.z-pos.z)) <= r * r ) {
            it->boom();
            return true;
        }
    }

    return false;

}

void Rockets::update(float t, float dt) {
    list<Rocket>::iterator it;

    // TODO - collision detection
    for (it=m_rockets.begin(); it != m_rockets.end();) {

        it->update(t, dt);

        // if ((glm::length(it->getPos()) > MAX_DIST) && it->flying()) {
        if (it->flying() &&
            ((glm::length(it->getPos()) > MAX_DIST) || it->pos.y < -1)) {

            it->boom();
            it++;
        } else if (it->dead()) {
            // it->~Rocket();
            m_rockets.erase(it++);
        }
        else {
            it++;
        }
    }
}

void Rockets::boomAll() {
    list<Rocket>::iterator it;
    for (it=m_rockets.begin(); it != m_rockets.end(); it++) {
        it->boom();
    }
}

// == render rocket
void Rockets::render() {
    list<Rocket>::iterator it;
    for (it=m_rockets.begin(); it != m_rockets.end(); it++) {
        it->render();
    }
}

void Rockets::add(vec3 p, vec3 v) {
    Rocket *r = new Rocket(p, v, rocketMesh, meshShader);
    // r->pos += v;

    m_rockets.push_back(*r);
    cout << "num: " << m_rockets.size()-1 << endl;
}

