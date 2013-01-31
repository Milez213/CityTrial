


#include "GameKartObject.h"
#include "GameUtilities.h"


#include <iostream>
using namespace std;

GamePhysicsActor *GameKartObject::makeKartActor(GamePhysics *physics)
{
   return physics->makeDynamicActor(physx::PxTransform(physx::PxVec3(0)), new physx::PxBoxGeometry(convert(glm::vec3(5.0))), physics->makeMaterial(), 5.0);
}
GamePhysicsActor *GameKartObject::makeTireActor(GamePhysics *physics)
{
   return physics->makeDynamicActor(physx::PxTransform(physx::PxVec3(0)), new physx::PxBoxGeometry(convert(glm::vec3(1.0))), physics->makeMaterial(), 1.0);
}

GameKartObject::GameKartObject(GamePhysics *physics, const char *fileName) : GameDrawableObject(makeKartActor(physics), "chassis") {
    
   for (int i = 0; i < 4; i++) {
      GameDrawableObject *tire = new GameDrawableObject(makeTireActor(physics), "tire");
      wheels.push_back(tire);
   }
   
   glm::vec3 pos = position();
   wheels[0]->setPosition(vec3(pos.x-5.0,pos.y-5.0,pos.z));
   wheels[1]->setPosition(vec3(pos.x - 5.0,pos.y + 5.0,pos.z));
   wheels[2]->setPosition(vec3(pos.x + 5.0,pos.y - 5.0,pos.z));
   wheels[3]->setPosition(vec3(pos.x + 5.0,pos.y + 5.0,pos.z));
    
   usingController = false;
    
   //object->setPosition(vec3(pos.x - 5.0,pos.y - 5.0,pos.z));
   
   /*&drawable_objects->push_back(wheel[1]);
   &drawable_objects->push_back(wheel[2]);
   &drawable_objects->push_back(wheel[3]);
   &drawable_objects->push_back(chassis);
   */

}

bool GameKartObject::collide(GamePhysicsActor *collide)
{
   return true;
}

GameKartObject::~GameKartObject()
{
   cout << "Kart Object Deleted\n";
}

void GameKartObject::stop()
{
   // cout << "bunnie stop\n";
   //stage = STILL;
}

void GameKartObject::draw(FlatShader *meshShader, RenderingHelper modelViewMatrix)
{
   
}

void GameKartObject::update(double dt)
{
    // Update Actor parameters based on current input from joystickState and buttonState
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
