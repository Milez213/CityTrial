//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameRamp.h"

#include "util.h"   // for rotate2d()



GameRamp::GameRamp() : GameSceneryObject("models/colorSkateboard.obj")
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
   
   vec3 pos = getPosition();
   vec3 rot = getRotation();
   vec3 scl = getScale();
   
   x -= pos.x;
   z -= pos.z;
   rotate2d(x, z, rot.y);
   x += pos.x;
   z += pos.z;
   
   float minHeight = pos.y - scl.y;
   float incHeight = scl.y*2;
   float minZ = pos.z - scl.z;
   float incZ = scl.z*2;
   float zMod = (z - minZ) / incZ;
   
   return minHeight + zMod*incHeight;
}

glm::vec3 GameRamp::getTopVectorAt(float x, float z) {
   vec3 scl(getScale()), rot(getRotation());
   
   float h = length(vec2(scl.x, scl.z));
   
   return normalize(vec3(h*cos(rot.y), scl.y, h*sin(rot.y)));
}
