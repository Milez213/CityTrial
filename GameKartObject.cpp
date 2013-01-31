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
   wheels[0]->setPosition(vec3(pos.x - 2.0,pos.y - 2.0,pos.z));
   wheels[1]->setPosition(vec3(pos.x - 2.0,pos.y + 2.0,pos.z));
   wheels[2]->setPosition(vec3(pos.x + 2.0,pos.y - 2.0,pos.z));
   wheels[3]->setPosition(vec3(pos.x + 2.0,pos.y + 2.0,pos.z));
    
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
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(position());
   modelViewMatrix.pushMatrix();
   meshShader->use();
   
   modelViewMatrix.translate(position());
   modelViewMatrix.scale(scl.x, scl.y, scl.z);
   modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
   modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
   modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
   meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   //glBindVertexArray(vertexArray);
   
   GLuint h_aPos = meshShader->getPosLocation();
   
   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.vertexBuffer);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   for (int i = 0; i < meshStorage.numMeshes; i++) {
      //printf("We are drawing, right? %d\n", indexBufferLength[i]);
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.indexBuffer[i]);
      
      glDrawElements(GL_TRIANGLES, meshStorage.indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
   }
   
   safe_glDisableVertexAttribArray(h_aPos);
   
   //glBindVertexArray(0);
   modelViewMatrix.popMatrix();

   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(-5.0,-5.0,0.0));
   
   wheels[0]->draw(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(-5.0,5.0,0.0));
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
   //if(usingController == true){
   if (joystickState[0] < 0.0) {
      glm::vec3 oldDir = direction();
      setDirection(glm::vec3(oldDir.x-(0.5 * dt),oldDir.y,oldDir.z));
   }
   if(joystickState[0] > 0.0) {
      glm::vec3 oldDir = direction();
      setDirection(glm::vec3(oldDir.x+(0.5 * dt),oldDir.y,oldDir.z));
   }
 
   if(joystickState[3] > 0.0) {
      float oldSpeed = speed();
      setSpeed(oldSpeed + (0.5 * dt));
   }
   if(joystickState[3] < 0.0) {
      float oldSpeed = speed();
      setSpeed(oldSpeed - (0.5 * dt));
   }
   //}
   

}
