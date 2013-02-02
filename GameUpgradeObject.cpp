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
}

void GameUpgradeObject::update(double time, double dt)
{
   setPosition(vec3(getPosition().x, getPosition().y + (cos(time) * 0.05), getPosition().z));
   
   float yRot = getRotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(getRotation().x, yRot, getPosition().z));
}