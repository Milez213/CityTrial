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

void GameUpgradeObject::update(float dt)
{
   setPosition(vec3(position().x, position().y + sin(dt), position().z));
   
   float yRot = rotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(rotation().x, yRot, position().z));
}