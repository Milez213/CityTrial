#include <GL/glfw.h>

#include "GameKartObject.h"
#include "GameSceneryObject.h"
#include "GameUpgradeObject.h"


#include "Upgrades/GamePartNone.h"
#include "Upgrades/GameActiveNone.h"

#include <cmath>

#include <iostream>
using namespace std;

extern SoundManager *g_sound_manager;
extern int g_num_squashes;

const float GameKartObject::tireTurnAngleTime = 1.0/4;

GameKartObject::GameKartObject(const char *fileName) : GamePhysicalObject("cube") {
   
   hud = new GameHUD();
    
   for (int i = 0; i < 4; i++) {
      GameDrawableObject *tire = new GameDrawableObject("models/tire.obj");
      wheels.push_back(tire);
   }
   
   /* fill each kartSlot with a blank upgrade. simplifies logic and allows
    * players to choose no part for a slot */
   GamePartUpgrade *part = new GamePartNone();
   frontParts.push_front(part);
   sideParts.push_front(part);
   backParts.push_front(part);
   activeUpgrades.push_front(new GameActiveNone());
    
   usingController = false;
   tireAngle = 0.0;
   tireTurnAngle = 0.0;
   carPitchAngle = 0.0;
   carRollAngle = 0.0;
   actionOn = false;
   points = 0;
   winState = 0;

   // load sounds
   ding_sound = g_sound_manager->getSample("sounds/ding.ogg");

   // made in sfxr
   activate_part_sound = g_sound_manager->getSample("sounds/select.wav");
   deactivate_part_sound = g_sound_manager->getSample("sounds/deselect.wav");

   // exaggerated sound
   collide_sound = g_sound_manager->getSample("sounds/crash.ogg");
   flying_sound = g_sound_manager->getSample("sounds/flying.ogg");
   outOfEnergy_sound = g_sound_manager->getSample("sounds/outofenergy.ogg");

   // to only play sound once per state change
   playedFlyingSound = false;
   pausedFlyingSound = true;
   playedOutOfEnergy = false; 
}

int GameKartObject::getInput(int request) {
   switch (request) {
      case 0:
         return input.up;
      case 1:
         return input.down;
      case 2:
         return input.left;
      case 3:
         return input.right;
      case 4:
         return input.action;
   }
   
   return -1;
}

void GameKartObject::onCollide(GameDrawableObject *other)
{
   //Need some way of telling if PhysicsActor came from upgrade
   
   if (GameUpgradeObject *upgrade =  dynamic_cast<GameUpgradeObject *>(other)) {
      upgrade->addToKart(this);
      points += 100;

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
            airborn = false;
            //fallSpeed = (oldPos.y-getRideHeight() - top)*abs(getSpeed());
            //setSpeed(oldSpeed + (oldSpeed > 0 ? fallSpeed : -fallSpeed));
         }
         else  {
            // bounce off
            collide_sound->play();
            setSpeed(-getSpeed());
            setPosition(oldPos);
         }
      } 
   }
   else if (GameKartObject *otherKart = dynamic_cast<GameKartObject *>(other)) {
      printf("collided with kart\n");
      vec3 oldPos = getPosition();
      setSpeed(-getSpeed());
      setPosition(oldPos);
   }
   else if (strcmp(other->getName(), "thingy") == 0) {
       // Collided with cuby thing
       ding_sound->play();

      other->setName("squashed_thingy");
      other->setScale(vec3(other->getScale().x, 0.02, other->getScale().z));
      points += 10;
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
   delete crash_sound;
   delete activate_sound;
   delete deactivate_sound;
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
   
   //rot.x = carRollAngle;

   //rot.z = carPitchAngle;
   //modelViewMatrix.rotate(carRollAngle,vec3(1.0,0.0,0.0));
   GameDrawableObject::drawSpecial(meshShader, modelViewMatrix,carPitchAngle,carRollAngle);
   

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
   modelViewMatrix.rotate(carPitchAngle,vec3(0.0,0.0,1.0));
   modelViewMatrix.rotate(-carRollAngle,vec3(1.0,0.0,0.0));  
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
   
   // draw parts
   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(2.0,0.0,0.0));
   frontParts.front()->drawOnKart(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();

   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(0.0,0.0,0.0));
   sideParts.front()->drawOnKart(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();

   modelViewMatrix.pushMatrix();
   modelViewMatrix.translate(glm::vec3(-2.0,0.0,0.0));
   backParts.front()->drawOnKart(meshShader,modelViewMatrix);
   modelViewMatrix.popMatrix();
   
   //draw action effect
   if (actionOn) {
      activeUpgrades.front()->drawEffect(meshShader, modelViewMatrix);
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

void GameKartObject::setHUDColor(vec3 color)
{
   hud->setColor(color);
}

void GameKartObject::drawHUD() {
   hud->prepareShader();
   hud->drawSpeed(getSpeed());
   hud->drawEnergy(getMaxEnergy(), getEnergy(), activeUpgrades.front()->getName());
   hud->drawScore();
   
   if (winState == 1) {
      hud->drawWin();
   } else if (winState == -1) {
      hud->drawLose();
   }
}


void GameKartObject::changeTireTurnAngle(float dt, float mult, float speedDampedTurnAngle)
{
   float targetAngle = mult*speedDampedTurnAngle;
   if (tireTurnAngle < targetAngle) {
      tireTurnAngle += dt * abs(speedDampedTurnAngle)/tireTurnAngleTime;
      if (tireTurnAngle > targetAngle)
         tireTurnAngle = targetAngle;
   }
   else if (tireTurnAngle > targetAngle) {
      tireTurnAngle -= dt * abs(speedDampedTurnAngle)/tireTurnAngleTime;
      if (tireTurnAngle < targetAngle)
         tireTurnAngle = targetAngle;
   }
}

void GameKartObject::changeKartPitchAngle(float dt, float pitchAngle)
{
   float targetAngle = pitchAngle;
   if(carPitchAngle < targetAngle)
   {
      carPitchAngle +=  dt * 10.0;
      if (carPitchAngle > targetAngle)
         carPitchAngle = targetAngle;
   }
   else if (carPitchAngle > targetAngle)
   {
      carPitchAngle -= dt * 10.0;
      if (carPitchAngle < targetAngle)
         carPitchAngle = targetAngle;
   }
}


void GameKartObject::changeKartRollAngle(float dt, float rollAngle)
{
   float targetAngle = rollAngle;
   if(carRollAngle < targetAngle)
   {
      carRollAngle += dt * 10.0;
      if (carRollAngle > targetAngle)
         carRollAngle = targetAngle;
   }
   else if (carRollAngle > targetAngle)
   {
      carRollAngle -= dt * 10.0;
      if (carRollAngle < targetAngle)
         carRollAngle = targetAngle;
   }
}

void GameKartObject::addPartToList(list<GamePartUpgrade *> &list, GamePartUpgrade *part)
{
   list.front()->cycleStatOff(&properties);
   list.push_front(part);
   part->cycleStatOn(&properties);
}
void GameKartObject::cyclePartList(list<GamePartUpgrade *> &list)
{
   list.front()->cycleStatOff(&properties);
   list.push_back(list.front());
   list.pop_front();
   list.front()->cycleStatOn(&properties);
   if (dynamic_cast<GamePartNone *>(list.front())) {
       deactivate_part_sound->play();
   }
}
void GameKartObject::addActive(GameActiveUpgrade *active)
{
   if (actionOn) {
      activeUpgrades.front()->activeEnd(this);
      actionOn = false;
   }
   activeUpgrades.push_front(active);
}
void GameKartObject::cycleActives()
{

   if (actionOn) {
      activeUpgrades.front()->activeEnd(this);
      actionOn = false;
   }
   activeUpgrades.push_back(activeUpgrades.front());
   activeUpgrades.pop_front();

   // empty or switch sound
   if (dynamic_cast<GameActiveNone *>(activeUpgrades.front())) {
       deactivate_part_sound->play();
   } else {
       activate_part_sound->play();
   }
}

void GameKartObject::win()
{
   winState = 1;
}

void GameKartObject::lose()
{
   winState = -1;
}

void GameKartObject::update(float dt)
{   
   float oldSpeed = getSpeed(), newSpeed;
   float oldDirection = getDirection();
   
   float speedDampedTurnAngle = properties.getTurnSpeed() * (1 - std::min(1.0f, abs(getSpeed())/properties.getTurnSpeed()));
   
   if (joystickState[0] < 0.0) {
      changeTireTurnAngle(dt, -1, speedDampedTurnAngle);
      //changeKartRollAngle(dt,-25.0);
   } else if(joystickState[0] > 0.0) {
      changeTireTurnAngle(dt, 1, speedDampedTurnAngle);
      //changeKartRollAngle(dt,25.0);
   } else if (joystickState[0] == 0.0){
      changeTireTurnAngle(dt, 0, speedDampedTurnAngle);
      //changeKartRollAngle(dt,0);
   }

   float directionMult = dt*tireTurnAngle * getSpeed()/M_PI;
   setDirection(oldDirection+directionMult); 
   setRotation(vec3(0, getDirection(), 0 ));
   
   
     


   if(joystickState[3] > 0.0 && oldSpeed < properties.getTopSpeed() && !isAirborn()) {
      short speedUp = (oldSpeed > 0.0) ? properties.getAcceleration() : properties.getBrakeSpeed();
      //changeKartPitchAngle(dt,25.0);
      newSpeed = oldSpeed + (speedUp * dt);
      if (newSpeed > properties.getTopSpeed()){
         newSpeed = properties.getTopSpeed();}
     
   } else if(joystickState[3] < 0.0 && !isAirborn()) {
      short speedDown = (oldSpeed < 0.0) ? properties.getAcceleration() : properties.getBrakeSpeed();
      //changeKartPitchAngle(dt,-25.0);
      newSpeed = oldSpeed - (speedDown * dt);
      if (newSpeed < -properties.getTopSpeed())
         newSpeed = -properties.getTopSpeed();
   } else {
      if (abs(oldSpeed) > friction * dt)
         newSpeed = oldSpeed - (sign(oldSpeed) * friction * dt);
         
      else {
         newSpeed = 0;
         //changeKartPitchAngle(dt, 0.0);
         //changeKartRollAngle(dt,0.0);
      }
   }
   

   
   setSpeed(newSpeed);
   
   
   
   if (buttonState[1] == GLFW_PRESS) { //inputMap.cycleActive
      if (!buttonDown[1]) {
         cycleActives();
         buttonDown[1] = true;
      }
   }
   else {
      buttonDown[1] = false;
   }
   if (buttonState[2] == GLFW_PRESS) { //inputMap.cycleFront
      if (!buttonDown[2]) {
         cycleFrontParts();
         buttonDown[2] = true;
      }
   }
   else {
      buttonDown[2] = false;
   }
   if (buttonState[3] == GLFW_PRESS) { //inputMap.cycleSide
      if (!buttonDown[3]) {
         cycleSideParts();
         buttonDown[3] = true;
      }
   }
   else {
      buttonDown[3] = false;
   }
   if (buttonState[4] == GLFW_PRESS) { //inputMap.cycleBack
      if (!buttonDown[4]) {
         cycleBackParts();
         buttonDown[4] = true;
      }
   }
   else {
      buttonDown[4] = false;
   }
   

   // be optimistic
   bool notEnoughEnergy = false; 

   if (buttonState[0] == GLFW_PRESS) { //inputMap.action
      if (!buttonDown[0]) {
         buttonDown[0] = true;
         
         notEnoughEnergy = !activeUpgrades.front()->activeStart(this);
         
         if (notEnoughEnergy) {
            properties.regenEnergy(dt);
         }
         else { // !notEnoughEnergy
            actionOn = true;
         }
      }
      else { // buttonDown[0]
         if (actionOn) {
            notEnoughEnergy = !activeUpgrades.front()->activeUpdate(this, dt);
            
            if (notEnoughEnergy) {
               activeUpgrades.front()->activeEnd(this);
               actionOn = false;
            }
            else { // !notEnoughEnergy
               
            }
         }
         else { // !actionOn
            properties.regenEnergy(dt);
         }
      }
   }
   else { // buttonState[0] == GLFW_RELEASE
      if (buttonDown[0]) {
         buttonDown[0] = false;
         
         // could play sound again after releasing action key
         playedOutOfEnergy = false;
         
         if (actionOn) {
            activeUpgrades.front()->activeEnd(this);
            actionOn = false;
         }
         else { // !actionOn
            properties.regenEnergy(dt);
         }
      }
      else { // !buttonDown[0]
         properties.regenEnergy(dt);
      }
   }

   if (notEnoughEnergy && !playedOutOfEnergy) {
       outOfEnergy_sound->play();
       playedOutOfEnergy = true;
   }

   
   // to only play/puse once per state change
   static int i = 0;

   // is flying?
   // from GamePhysicalObject::update()
   if (isAirborn()) {
       /*if(getFallSpeed() < 0)
          changeKartPitchAngle(dt,10.0);
       else if (getFallSpeed() > 0)
          changeKartPitchAngle(dt,-10.0);
       else*/
          changeKartPitchAngle(dt,-getFallSpeed());
      
       if(joystickState[0] > 0.0)
          changeKartRollAngle(dt,25.0);
       else if(joystickState[0] < 0.0)
          changeKartRollAngle(dt,-25.0);
       else
          changeKartRollAngle(dt,0.0);

       // flying_sound->resume();
       if (!playedFlyingSound) {
           // printf("play %d\n", i++);
           flying_sound->fadeIn(1000, -1); // fadin and loop forever
           playedFlyingSound = true;
           pausedFlyingSound = false;
       }
   } else {
      carPitchAngle = 0;
      carRollAngle = 0;
      //changeKartPitchAngle(dt,0.0);
      //changeKartRollAngle(dt,0.0);
       if (!pausedFlyingSound) {
        
           // printf("paused %d\n", i);
           flying_sound->fadeOut(1000);
           pausedFlyingSound = true;
           playedFlyingSound = false;
       }
   }
   
   
   // do this last
   airborn = true;
   GamePhysicalObject::update(dt); //actually move the cart with these updated values
}
