#include "GameKartObject.h"
#include "GameUtilities.h"


#include <iostream>
using namespace std;

extern GamePhysics *g_physics;

GamePhysicsActor *GameKartObject::makeKartActor()
{
   return g_physics->makeDynamicActor(physx::PxTransform(physx::PxVec3(0, 0, 5)), new physx::PxBoxGeometry(convert(glm::vec3(5.0))), g_physics->makeMaterial(), 5.0);
}
GamePhysicsActor *GameKartObject::makeTireActor()
{
   return g_physics->makeDynamicActor(physx::PxTransform(physx::PxVec3(0)), new physx::PxBoxGeometry(convert(glm::vec3(1.0))), g_physics->makeMaterial(), 1.0);
}

GameKartObject::GameKartObject(const char *fileName) : GameDrawableObject(makeKartActor(), "chassis") {
    
   for (int i = 0; i < 4; i++) {
      GameDrawableObject *tire = new GameDrawableObject(makeTireActor(), "tire");
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
   glm::vec3 pos = position();
 
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(pos.x,pos.y,pos.z));
    //modelViewMatrix.scale(glm::vec3(0.5,0.5,0.5));
    //Draw itself
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(-2.0,-2.0,0.0));
    
    wheels[0]->draw(meshShader,modelViewMatrix);
    modelViewMatrix.popMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(5.0,-5.0,0.0));
    wheels[1]->draw(meshShader,modelViewMatrix);
    modelViewMatrix.popMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(5.0,-5.0,0.0));
    wheels[2]->draw(meshShader,modelViewMatrix);
    modelViewMatrix.popMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(5.0,5.0,0.0));
    wheels[3]->draw(meshShader,modelViewMatrix);
    modelViewMatrix.popMatrix();
    modelViewMatrix.popMatrix();  
 


}

void GameKartObject::update(double dt)
{
    // Update Actor parameters based on current input from joystickState and buttonState
   if(usingController == true){
    if (joystickState[0] < 0.0)
    {
    glm::vec3 oldDir = direction();
    setDirection(glm::vec3(oldDir.x-(0.5 * dt),oldDir.y,oldDir.z));
    }  

    if(joystickState[0] > 0.0)
    {
    glm::vec3 oldDir = direction();
    setDirection(glm::vec3(oldDir.x+(0.5 * dt),oldDir.y,oldDir.z));   
    }
 
    if(joystickState[3] > 0.0)
    {
    float oldSpeed = speed();    
    setSpeed(oldSpeed + (0.5 * dt));
    }
    if(joystickState[3]<0.0)
    {
    float oldSpeed = speed();    
    setSpeed(oldSpeed - (0.5 * dt));    
    }
   }
   

}
