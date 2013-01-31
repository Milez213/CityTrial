



#include "GameKartObject.h"

#include <iostream>
using namespace std;

GameKartObject::GameKartObject(const char *fileName) : GameDrawableObject("chassis") {
    
   for (int i = 0; i < 4; i++) {
      GameDrawableObject *tire = new GameDrawableObject("tire");
      wheels.push_back(tire);
   }
    
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
   
 
    modelViewMatrix.useMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(pos.x,pos.y,pos.z));
    modelViewMatrix.rotate(glm::vec3(rot.x,rot.y,rot.z));
    chassis.draw(meshShader,modelViewMatrix);
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(-5.0,-5.0,0.0));
    wheel[0].draw(meshShader,modelViewMatrix);
    modelViewmatrix.popMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(-5.0,5.0,0.0));
    wheel[1].draw(meshShader,modelViewMatrix);
    modelViewmatrix.popMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(5.0,-5.0,0.0));
    wheel[2].draw(meshShader,modelViewMatrix);
    modelViewmatrix.popMatrix();
    modelViewMatrix.pushMatrix();
    modelViewMatrix.translate(glm::vec3(5.0,5.0,0.0));
    wheel[3].draw(meshShader,modelViewMatrix);
    modelViewmatrix.popMatrix();  
    modelViewMatrix.popMatrix();  
 


}

void GameKartObject::update(double dt)
{
    // Update Actor parameters based on current input from joystickState and buttonState
   if(usingController == true){
    if (joystickState[0] < 0.0)
    {
       //rotate actor left
    }  

    if(joystickState[0] > 0.0)
    {
       //rotate actor right
    }
 
    if(joystickState[3] > 0.0)
    {
    //accelerate
    }
    if(joystickState[3]<0.0)
    {
    //decelerate 
    }
   }
   

}
