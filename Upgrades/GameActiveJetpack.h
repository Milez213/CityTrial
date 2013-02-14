//
//  GameActiveJetpack.h
//  
//
//  Created by Eric Johnson on 2/11/13.
//
//

#ifndef _GameActiveJetpack_h
#define _GameActiveJetpack_h

#include "../GameActiveUpgrade.h"
#include "../defines.h"

class GameActiveJetpack : public GameActiveUpgrade
{
public:
   GameActiveJetpack() : GameActiveUpgrade("jetpack") {}
   
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   glm::vec3 getVecFromDir(float dir) { dir = TO_RADIANS(-dir); return glm::vec3(cos(dir), 0, sin(dir)); }
   static const float STRAFE_SPEED = 10.0;
   
   virtual bool activeStart(GameKartObject *kart) {return true;}
   virtual bool activeUpdate(GameKartObject *kart, float dt)
   {
      if (kart->properties.useEnergyDrain(50, dt)) {
         kart->setFallSpeed(kart->getFallSpeed()-30*dt);
         
         if(kart->getJoystickState(0) > 0.0) {
            kart->setPosition(kart->getPosition() + dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()+90));
            kart->changeKartRollAngle(dt,25.0);
         }
         else if (kart->getJoystickState(0) < 0.0) {
            kart->setPosition(kart->getPosition() + dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()-90));
            kart->changeKartRollAngle(dt,-25.0);
         }
         else {
            kart->changeKartRollAngle(dt,0.0);
         }
         
         kart->changeKartPitchAngle(dt, kart->getFallSpeed());
         if(kart->getJoystickState(3) > 0.0) {
            kart->setPosition(kart->getPosition() + dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()));
            kart->changeKartPitchAngle(dt,-10.0);
         }
         else if (kart->getJoystickState(3) < 0.0) {
            kart->setPosition(kart->getPosition() - dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()));
            kart->changeKartPitchAngle(dt,10.0);
         }
         else {
            kart->changeKartPitchAngle(dt,0.0);
         }
            
         
         return true;
      }
      return false;  
   }
   virtual void activeEnd(GameKartObject *kart) {}
};

#endif
