//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameRamp.h"

#include "GameUtilities.h"

extern GamePhysics *g_physics;

static GamePhysicsActor *makeRampActor()
{
   return g_physics->makeDynamicActor(physx::PxTransform(physx::PxVec3(0)), new physx::PxBoxGeometry(convert(glm::vec3(5.0))), g_physics->makeMaterial(), 5.0);
}

GameRamp::GameRamp() : GameDrawableObject(makeRampActor() ,"ramp")
{
   
}

float GameRamp::getHeightAt(float x, float z)
{
   //float xMod = (x - pos.x) / scl.x;
   float zMod = (z - position().z) / scl.z;
   
   zMod += 1.0;
   
   return zMod * scl.y;
}