//
//  GameUpgradeObject.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameUpgradeObject.h"

extern SoundManager *g_sound_manager;

/*GameUpgradeObject::GameUpgradeObject(Type initType) : GameDrawableObject("cube")
{
   type = initType;
   name = "upgrade";
   toRemove = false;
}*/
GameUpgradeObject::GameUpgradeObject(const char *objFile) : GameDrawableObject(objFile)
{
    pickup_sound =  g_sound_manager->getSample("sounds/generic_pickup.ogg");
}

void GameUpgradeObject::update(float dt)
{
   //setPosition(vec3(getPosition().x, getPosition().y + 0.01 * cos(time), getPosition().z));
   
   float yRot = getRotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(getRotation().x, yRot, getPosition().z));
}

void GameUpgradeObject::onCollide(GameDrawableObject *other)
{
   if (GameKartObject *kart =  dynamic_cast<GameKartObject *>(other)) {
      playPickupSound();
      scheduleForRemoval();
   }
}

void GameUpgradeObject::playPickupSound() {
    pickup_sound->play();
}


