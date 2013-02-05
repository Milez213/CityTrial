//
//  GameUpgradeObject.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameUpgradeObject.h"
#include "GameKartObject.h"


GameUpgradeObject::GameUpgradeObject(Type initType) : GameDrawableObject("cube")
{
   type = initType;
   name = "upgrade";
   toRemove = false;
}

void GameUpgradeObject::update(double time, double dt)
{
   setPosition(vec3(getPosition().x, getPosition().y /* + 0.1 * cos(time)*/, getPosition().z));
   
   float yRot = getRotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   // setRotation(vec3(getRotation().x, yRot, getPosition().z));
}

void GameUpgradeObject::onCollide(GameDrawableObject *other)
{
   if (GameKartObject *upgrade =  dynamic_cast<GameKartObject *>(other)) {
      scheduleForRemoval();
   }
}
