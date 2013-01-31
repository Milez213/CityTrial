//
//  GameUpgradeObject.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameUpgradeObject.h"

GameUpgradeObject::GameUpgradeObject(GamePhysicsActor *actor, Type initType) : GameDrawableObject(actor, "cube")
{
   type = initType;
}

void GameUpgradeObject::update(double time, double dt)
{
   setPosition(vec3(position().x, position().y + (cos(time) * 0.05), position().z));
   
   float yRot = rotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(rotation().x, yRot, position().z));
}