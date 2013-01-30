


#include "GameKartObject.h"

#include <iostream>
using namespace std;


float randFloat() {
    return ((float) rand() / RAND_MAX);
}


GameKartObject::GameKartObject(glm::vec3 p, glm::vec3 v, Mesh* rmesh, FlatShader* inmeshShader) {
    pos = p;
    //vel = glm::vec3(1.0,0.0,0.0);

    mesh = rmesh;

    stage = MOVING;
    meshShader = inmeshShader;

    // add(vec3(-10 + 20 * randFloat(), 5 + 5 * randFloat(), -30), vec3(0, 0, 1));
    //s = scale(mat4(1.0f), vec3(2));
    r = rotate(mat4(1.0f), randFloat() * 360, vec3(0, 1, 0));
    

}


// Does the bunnie collide with a sphere at pos, radius r?
bool GameKartObject::collide(glm::vec3 pos, float r) {
    if ( ((this->pos.x-pos.x) * (this->pos.x-pos.x) +
                (this->pos.y-pos.y) * (this->pos.y-pos.y) +
                (this->pos.z-pos.z) * (this->pos.z-pos.z)) <= r * r ) {
        return true;
    }
    return false;
}

GameKartObject::~GameKartObject() {
    cout << "Bunnie deleted\n";
}

void GameKartObject::stop() {
    // cout << "bunnie stop\n";
    //stage = STILL;
}


vec3 GameKartObject::getPos() {
    return pos;
}

void GameKartObject::draw() {
    mat4 t = translate(mat4(1.0f), pos);

    switch (stage) {

    case MOVING:
        break;

    case STILL:
        // TODO change material?

        break;

    case DONE:
        return;
        break;

    default:

        break;
    }
    // render mesh
    meshShader->use();
    meshShader->setModelMatrix(t * s * r);

    mesh->draw(meshShader);
}

void GameKartObject::update(double dt) {
    switch (stage) {

    case MOVING:
        // pos += vel * (BUNNIE_SPEED * dt * MS_2_S);
        r = rotate(r,  10 * (float) dt  , vec3(1, 0, 0));
        
        break;

    case STILL:
        break;

    case DONE:
        break;

    default:
        break;
    }
}


/*
void Bunnie::stop() {
    if (stage == FLYING) {
        stage = TRAIL;
        // TODO - make black
        smoke = new TFBParticles(g_time * MS_2_S);
        smoke->init(pos);
    }
}
*/
