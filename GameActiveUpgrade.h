//
//  GameActiveUpgrade.h
//  
//
//  Created by Eric Johnson on 2/10/13.
//
//

#ifndef ____GameActiveUpgrade__
#define ____GameActiveUpgrade__

#include <iostream>
#include "GameUpgradeObject.h"

class GameActiveUpgrade : public GameUpgradeObject
{
public:
   GameActiveUpgrade() : GameUpgradeObject("ramp") {}
   
   virtual void addToKart(GameKartObject *kart) { kart->addActive(this); }
   
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) = 0;
   virtual void activeStart(GameKartObject *kart) = 0;
   virtual void activeUpdate(GameKartObject *kart, float dt) = 0;
   virtual void activeEnd(GameKartObject *kart) = 0;
};

#endif /* defined(____GameActiveUpgrade__) */
