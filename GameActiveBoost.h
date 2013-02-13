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
#include "SoundManager.h"
#include <iostream>

extern SoundManager *g_sound_manager;

class GameActiveBoost : public GameActiveUpgrade
{
public:
   GameActiveBoost() : GameActiveUpgrade() {
       activate_sound = g_sound_manager->getSample("sounds/rocket_fire.ogg");
   }
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   virtual void activeStart(GameKartObject *kart)
   {
      if (kart->properties.useEnergyCost(25)) {
         kart->setSpeed(kart->getSpeed()+10);
         activate_sound->play();
      }
   }
   virtual void activeUpdate(GameKartObject *kart, float dt) {}
   virtual void activeEnd(GameKartObject *kart) {}

private:
   GameSound *activate_sound;
};

#endif
