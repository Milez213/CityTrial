#include "GameKartObject.h"
#include "GamePartUpgrade.h"
#include "GameSceneryObject.h"
#include "GamePartWings.h"

#include <cmath>

#include <iostream>
using namespace std;

extern SoundManager *g_sound_manager;
extern int g_num_squashes;


//const float GameKartObject::maxTireTurnAngle = 45.0;
const float GameKartObject::tireTurnAngleTime = 1.0/4;

GameKartObject::GameKartObject(const char *fileName) : GamePhysicalObject("cube") {
    
   for (int i = 0; i < 4; i++) {
      GameDrawableObject *tire = new GameDrawableObject("models/tire.obj");
      wheels.push_back(tire);
   }
      GamePartUpgrade *part = new GamePartWings();
      parts.push_back(part);
   /*glm::vec3 pos = position();
   wheels[0]->setPosition(vec3(pos.x - 12.0, pos.y-6, pos.z - 12.0));
   wheels[1]->setPosition(vec3(pos.x + 12.0, pos.y-6, pos.z - 12.0));
   wheels[2]->setPosition(vec3(pos.x + 12.0, pos.y-6, pos.z + 12.0));
   wheels[3]->setPosition(vec3(pos.x - 12.0, pos.y-6, pos.z + 12.0));*/
    
   usingController = false;
   tireAngle = 0.0;
   tireTurnAngle = 0.0;
    
   //object->setPosition(vec3(pos.x - 5.0,pos.y - 5.0,pos.z));
   
   /*&drawable_objects->push_back(wheel[1]);
   &drawable_objects->push_back(wheel[2]);
   &drawable_objects->push_back(wheel[3]);
   &drawable_objects->push_back(chassis);
   */

   ding_sound = g_sound_manager->getSample("sounds/ding.ogg");

   // collide_sound = g_sound_manager->getSample("sounds/157609__qubodup__hollow-bang.wav");
}

void GameKartObject::onCollide(GameDrawableObject *other)
{
   //Need some way of telling if PhysicsActor came from upgrade
   
   if (GameUpgradeObject *upgrade =  dynamic_cast<GameUpgradeObject *>(other)) {
      upgrade->apply(&properties);
      /*if (upgrade->upgradeType() == GameUpgradeObject::FLIGHT) {
         properties.setWings();
         GameUpgradeObject *upgrade = dynamic_cast<GameUpgradeObject *>(other);
         
         // assert upgrade != NULL
         
         upgrade->scheduleForRemoval();
      }
      else {
         
      }*/

   }
   else if (GameSceneryObject *scenery =  dynamic_cast<GameSceneryObject *>(other)) {
      
      vec3 oldPos = getPosition();
      //float oldSpeed = getSpeed();
      float top = scenery->getHeightAt(oldPos.x, oldPos.z);
      float bottom = scenery->getBottomAt(oldPos.x, oldPos.z);
      
      if (oldPos.y - getRideHeight() < top and oldPos.y + getRideHeight() > bottom) {//and oldPos.y + 1 >= newHeight) {
         if (oldPos.y - getRideHeight() + 1.5 > top) {
            setPosition(vec3(oldPos.x, top+getRideHeight(), oldPos.z));
            fallSpeed = 0;
            //fallSpeed = (oldPos.y-getRideHeight() - top)*abs(getSpeed());
            //setSpeed(oldSpeed + (oldSpeed > 0 ? fallSpeed : -fallSpeed));
         }
         else  {
            setSpeed(0);
         }
      }
   }
   else if (strcmp(other->getName(), "thingy") == 0) {
       // Collided with cuby thing
       ding_sound->play();

      other->setName("squashed_thingy");
      other->setScale(vec3(other->getScale().x, 0.02, other->getScale().z));
       g_num_squashes++;
   }
   else {
      GamePhysicalObject::onCollide(other);
   }
   
   //return true;
}

GameKartObject::~GameKartObject()
{

#ifdef USE_SOUND 
   delete ding_sound;
#endif
   cout << "Kart Object Deleted\n";
}

/*void GameKartObject::stop()
{
   // cout << "bunnie stop\n";
   //stage = STILL;
}*/

void GameKartObject::draw(PhongShader *meshShader, RenderingHelper modelViewMatrix)
{
   tireAngle+=(getSpeed()/2.0);
   GameDrawableObject::draw(meshShader, modelViewMatrix);
   
   
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(getPosition());
   modelViewMatrix.scale(0.5,0.5,0.5);
  
   
   //modelViewMatrix.pushMatrix();
   //meshShader->use();
   
   //modelViewMatrix.translate(position());
   //modelViewMatrix.scale(scl.x, scl.y, scl.z);
   modelViewMatrix.rotate(rot.x, vec3(1.0, 0.0, 0.0));
   modelViewMatrix.rotate(rot.y, vec3(0.0, 1.0, 0.0));
   modelViewMatrix.rotate(rot.z, vec3(0.0, 0.0, 1.0));
   //meshShader->setModelMatrix(modelViewMatrix.getMatrix());
   
   //glBindVertexArray(vertexArray);
   
   //GLuint h_aPos = meshShader->getPosLocation();
   
   /*safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, meshStorage.vertexBuffer);
   safe_glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   for (int i = 0; i < meshStorage.numMeshes; i++) {
      //printf("We are drawing, right? %d\n", indexBufferLength[i]);
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshStorage.indexBuffer[i]);
      
      glDrawElements(GL_TRIANGLES, meshStorage.indexBufferLength[i], GL_UNSIGNED_SHORT, 0);
   }*/
   
   //safe_glDisableVertexAttribArray(h_aPos);
   
   //glBindVertexArray(0);
   //modelViewMatrix.popMatrix();
   
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(-2.2,0.0,-2.2));
   modelViewMatrix.rotate(90.0,vec3(0.0,1.0,0.0));
   modelViewMatrix.rotate(tireAngle,vec3(1.0,0.0,0.0));
   modelViewMatrix.scale(5.0,6.0,6.0);
   wheels[0]->draw(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(-2.2,0.0,2.2));
 modelViewMatrix.rotate(90.0,vec3(0.0,1.0,0.0));
   modelViewMatrix.rotate(tireAngle,vec3(1.0,0.0,0.0));
   modelViewMatrix.scale(5.0,6.0,6.0);
   wheels[1]->draw(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(2.2,0.0,-2.2));
 modelViewMatrix.rotate(90.0,vec3(0.0,1.0,0.0));
   modelViewMatrix.rotate(tireTurnAngle,vec3(0.0,1.0,0.0));
   modelViewMatrix.rotate(tireAngle,vec3(1.0,0.0,0.0));   
   modelViewMatrix.scale(5.0,6.0,6.0);
   wheels[2]->draw(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(2.2,0.0,2.2));
 modelViewMatrix.rotate(90.0,vec3(0.0,1.0,0.0));
   modelViewMatrix.rotate(tireTurnAngle,vec3(0.0,1.0,0.0));
   modelViewMatrix.rotate(tireAngle,vec3(1.0,0.0,0.0));

   modelViewMatrix.scale(5.0,6.0,6.0);
   wheels[3]->draw(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();
   if (properties.hasWings())
   {
      modelViewMatrix.pushMatrix();
      modelViewMatrix.translate(glm::vec3(0.0,0.0,0.0));
      modelViewMatrix.scale(0.1,0.1,5.0);
      modelViewMatrix.rotate(80.0,vec3(0.0,0.0,1.0));
      parts[0]->draw(meshShader,modelViewMatrix);
      modelViewMatrix.popMatrix();
   }


   modelViewMatrix.popMatrix();
   
   //Draws Wheels and Upgrades More efficiently *****
   /*for (int i = 0; i < (int)wheels.size(); i++) {
      modelViewMatrix.pushMatrix();
      wheels[i]->draw(meshShader,modelViewMatrix);
      modelViewMatrix.popMatrix();
   }
   
   for (int i = 0; i < (int)upgrades.size(); i++) {
      modelViewMatrix.pushMatrix();
      upgrades[i]->draw(meshShader,modelViewMatrix);
      modelViewMatrix.popMatrix();
   }*/
}


void GameKartObject::changeTireTurnAngle(float dt, float targetAngle)
{
   if (tireTurnAngle < targetAngle) {
      tireTurnAngle += dt * properties.getTurnSpeed()/tireTurnAngleTime;
      if (tireTurnAngle > targetAngle)
         tireTurnAngle = targetAngle;
   }
   else if (tireTurnAngle > targetAngle) {
      tireTurnAngle -= dt * properties.getTurnSpeed()/tireTurnAngleTime;
      if (tireTurnAngle < targetAngle)
         tireTurnAngle = targetAngle;
   }
   
   /*if (targetAngle < 0.0)
   {
      if(tireTurnAngle > targetAngle)
       tireTurnAngle -= dt * properties.getTurnSpeed()/tireTurnAngleTime;
   }
   else if(targetAngle > 0.0)
   {
      if(tireTurnAngle < targetAngle)
         tireTurnAngle += dt * properties.getTurnSpeed()/tireTurnAngleTime;
   }
   else
   {
      if(tireTurnAngle > 0.0)
         tireTurnAngle -= dt * properties.getTurnSpeed()/tireTurnAngleTime;
      else if(tireTurnAngle<0.0)
         tireTurnAngle += dt * properties.getTurnSpeed()/tireTurnAngleTime;
   }*/

}

void GameKartObject::update(float dt)
{
   glm::vec3 vel = getVelocity();
   glm::vec3 pos = getPosition();
   
   /*
   printf("\nbefore\n");
   printf(" position: %f,%f,%f\n", pos.x, pos.y, pos.z);
   printf(" velocity: %f,%f,%f\n", vel.x, vel.y, vel.z);
   printf(" direction: %f\n", getDirection());
   printf(" speed: %f\n", getSpeed());
   */
   
    // Update Actor parameters based on current input from joystickState and buttonState
   //if(usingController == true){
   
   /*vec3 up = vec3(0.0, 1.0, 0.0);
   glm::vec3 oldDir = direction();
   vec3 move = normalize(cross(up, oldDir));
   move = vec3(move.x * turningRadius * dt, move.y * turningRadius * dt, move.z * turningRadius * dt);*/
   
   float oldSpeed = getSpeed(), newSpeed;
   float oldDirection = getDirection();
   
   //float directionMult = oldSpeed == 0 ? 0 : (oldSpeed < 0.0) ? -1 : 1;
   float speedDampedTurnAngle = properties.getTurnSpeed() * (1 - abs(getSpeed())/properties.getTurnSpeed());
   
   if (joystickState[0] < 0.0) {
      changeTireTurnAngle(dt, -speedDampedTurnAngle);
      //setDirection(glm::vec3(oldDir.x - move.x,oldDir.y,oldDir.z - move.z));
   } else if(joystickState[0] > 0.0) {
      changeTireTurnAngle(dt, speedDampedTurnAngle);
      //setDirection(glm::vec3(oldDir.x + move.x,oldDir.y,oldDir.z + move.z));
   } else if (joystickState[0] == 0.0){
      changeTireTurnAngle(dt, 0.0);
   }

   
   float directionMult = dt*tireTurnAngle * getSpeed()/M_PI;
   setDirection(oldDirection+directionMult);
 
   setRotation(vec3(0, getDirection(), 0 ));
   
   
   

   
   if(joystickState[3] > 0.0) {
      short speedUp = (oldSpeed > 0.0) ? properties.getAcceleration() : properties.getBrakeSpeed();
      newSpeed = oldSpeed + (speedUp * dt);
   } else if(joystickState[3] < 0.0) {
      short speedDown = (oldSpeed < 0.0) ? properties.getAcceleration() : properties.getBrakeSpeed();
      newSpeed = oldSpeed - (speedDown * dt);
   } else {
      if (abs(oldSpeed) > friction * dt)
         newSpeed = oldSpeed - (sign(oldSpeed) * friction * dt);
      else
         newSpeed = 0;
   }
   
   if (newSpeed > properties.getTopSpeed())
      newSpeed = properties.getTopSpeed();
   else if (newSpeed < -properties.getTopSpeed())
         newSpeed = -properties.getTopSpeed();
   
   setSpeed(newSpeed);
   
   //}
   
   vel = getVelocity();
   pos = getPosition();
   
   /*
   printf("after\n");
   printf(" position: %f,%f,%f\n", pos.x, pos.y, pos.z);
   printf(" velocity: %f,%f,%f\n", vel.x, vel.y, vel.z);
   printf(" direction: %f\n", getDirection());
   printf(" speed: %f\n", getSpeed());
   */
   
   GamePhysicalObject::update(dt); //actually move the cart with these updated values
}
