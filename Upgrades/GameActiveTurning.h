//
//  GameActiveTurning.h
//  
//
//  Created by Eric Johnson on 2/13/13.
//
//

#ifndef _GameActiveTurning_h
#define _GameActiveTurning_h

#include "../GameActiveUpgrade.h"

class GameActiveTurning : public GameActiveUpgrade
{
public:
   GameActiveTurning() : GameActiveUpgrade("turning") {}
   
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   virtual bool activeStart(GameKartObject *kart)
   {
      kart->properties.upgradeTurnSpeed(3);
      return true;
   }
   virtual bool activeUpdate(GameKartObject *kart, float dt)
   {
      return kart->properties.useEnergyDrain(25, dt);
   }
   virtual void activeEnd(GameKartObject *kart)
   {
      kart->properties.downgradeTurnSpeed(3);
   }
};

#endif
