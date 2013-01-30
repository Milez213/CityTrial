


#include "GameKartObject.h"


#include <iostream>
using namespace std;


float randFloat() {
    return ((float) rand() / RAND_MAX);
}

GameKartObject::GameKartObject(glm::vec3 p, FlatShader* inmeshShader) {
    pos = p;
    //vel = glm::vec3(1.0,0.0,0.0);

    //mesh = rmesh;

    stage = MOVING;
    meshShader = inmeshShader;

    // add(vec3(-10 + 20 * randFloat(), 5 + 5 * randFloat(), -30), vec3(0, 0, 1));
    //s = scale(mat4(1.0f), vec3(2));
    r = rotate(mat4(1.0f), randFloat() * 360, vec3(0, 1, 0));
    
    

   
    chassis = new GameDrawableObject("Stuff");
 wheel[0] = new GameDrawableObject("Stuff2");
 wheel[1] = new GameDrawableObject("Stuff3");
 wheel[2] = new GameDrawableObject("Stuff4");
 wheel[3] = new GameDrawableObject("Stuff5");
    
    wheel[0]->setPosition(vec3(pos.x-5.0,pos.y-5.0,pos.z));
    wheel[1]->setPosition(vec3(pos.x - 5.0,pos.y + 5.0,pos.z)); 
    wheel[2]->setPosition(vec3(pos.x + 5.0,pos.y - 5.0,pos.z));
    wheel[3]->setPosition(vec3(pos.x + 5.0,pos.y + 5.0,pos.z));
    chassis->setPosition(vec3(pos.x,pos.y,pos.z));
    

    
        //object->setPosition(vec3(pos.x - 5.0,pos.y - 5.0,pos.z));
   
    /*&drawable_objects->push_back(wheel[1]);
    &drawable_objects->push_back(wheel[2]);
    &drawable_objects->push_back(wheel[3]);
    &drawable_objects->push_back(chassis);  
    */

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
    //meshShader->use();
    //meshShader->setModelMatrix(t * s * r);

    //mesh->draw(meshShader);
}

void GameKartObject::update(double dt) {
    switch (stage) {

    case MOVING:
           
        // pos += vel * (BUNNIE_SPEED * dt * MS_2_S);
        r = rotate(r,  10 * (float) dt  , vec3(1, 0, 0));
            meshShader->use();
    meshShader->setModelMatrix( r);
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
