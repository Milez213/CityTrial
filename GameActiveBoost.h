//
//  GameActiveBoost.h
//  
//
//  Created by Eric Johnson on 2/10/13.
//
//

#ifndef _GameActiveBoost_h
#define _GameActiveBoost_h

#include "GameActiveUpgrade.h"
#include <iostream>

class GameActiveBoost : public GameActiveUpgrade
{
public:
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   virtual void activeStart(GameKartObject *kart)
   {
      cout << "activeStart()\n";
      if (kart->properties.useEnergyCost(25))
         kart->setSpeed(kart->getSpeed()+10);
   }
   virtual void activeUpdate(GameKartObject *kart, float dt) {}
   virtual void activeEnd(GameKartObject *kart) {}
};

#endif
