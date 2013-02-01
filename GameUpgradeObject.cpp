//
//  GameUpgradeObject.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameUpgradeObject.h"
#include "GameUtilities.h"

extern GamePhysics *g_physics;

GamePhysicsActor *GameUpgradeObject::makeUpgradeActor(glm::vec3 position)
{
   physx::PxRigidDynamic *actor = g_physics->makeBlankDynamic(physx::PxTransform(convert(position)));
   actor->createShape(*new physx::PxBoxGeometry(convert(glm::vec3(1.0, 1.0, 1.0))), *g_physics->makeMaterial());
   actor->setMass(0.0);
   actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
   
   return g_physics->makeActor(actor);
}

GameUpgradeObject::GameUpgradeObject(Type initType, glm::vec3 position) : GameDrawableObject(makeUpgradeActor(position), "cube")
{
   type = initType;
   name = "upgrade";
}

void GameUpgradeObject::update(double time, double dt)
{
   setPosition(vec3(position().x, position().y + (cos(time) * 0.05), position().z));
   
   float yRot = rotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(rotation().x, yRot, position().z));
}