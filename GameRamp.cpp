//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameRamp.h"

#include "util.h"   // for rotate2d()



GameRamp::GameRamp() : GameSceneryObject("ramp")
{
   
}

float GameRamp::getHeightAt(float x, float z)
{
   //float xMod = (x - pos.x) / scl.x;
   /*float zMod = (z - getPosition().z) / scl.z;
   
   zMod += 1.0;
   
   return zMod * scl.y;*/

   // account for rotation of th around y axis
   // rotate x and z
   rotate2d(x, z, rot.y);
   
   vec3 pos = getPosition();
   float minHeight = pos.y - scl.y;
   float incHeight = scl.y*2;
   float minZ = pos.z - scl.z;
   float incZ = scl.z*2;
   float zMod = (z - minZ) / incZ;
   
   return minHeight + zMod*incHeight;
}
