//
//  GameUpgradeObject.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameUpgradeObject.h"


GameUpgradeObject::GameUpgradeObject(Type initType) : GameDrawableObject("cube")
{
   type = initType;
   name = "upgrade";
   toRemove = false;
}

void GameUpgradeObject::update(double time, double dt)
{
   setPosition(vec3(getPosition().x, 1 + 0.5 * cos(time), getPosition().z));
   
   float yRot = getRotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(getRotation().x, yRot, getPosition().z));
}
