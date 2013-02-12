//
//  GameActiveJetpack.h
//  
//
//  Created by Eric Johnson on 2/11/13.
//
//

#ifndef _GameActiveJetpack_h
#define _GameActiveJetpack_h

#include "GameActiveUpgrade.h"

class GameActiveJetpack : public GameActiveUpgrade
{
public:
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   virtual void activeStart(GameKartObject *kart) {}
   virtual void activeUpdate(GameKartObject *kart, float dt)
   {
      if (kart->properties.useEnergyDrain(50, dt))
         kart->setFallSpeed(kart->getFallSpeed()-30*dt);
   }
   virtual void activeEnd(GameKartObject *kart) {}
};

#endif
