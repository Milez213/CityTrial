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
         }
         else if (kart->getJoystickState(0) < 0.0) {
            kart->setPosition(kart->getPosition() + dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()-90));
         }
         
         if(kart->getJoystickState(3) > 0.0) {
            kart->setPosition(kart->getPosition() + dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()));
         }
         else if (kart->getJoystickState(3) < 0.0) {
            kart->setPosition(kart->getPosition() - dt*STRAFE_SPEED*getVecFromDir(kart->getDirection()));
         }
            
         
         return true;
      }
      return false;  
   }
   virtual void activeEnd(GameKartObject *kart) {}
};

#endif
