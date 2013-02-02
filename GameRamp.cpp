//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameRamp.h"

GameRamp::GameRamp() : GameDrawableObject("ramp")
{
   
}

float GameRamp::getHeightAt(float x, float z)
{
   //float xMod = (x - pos.x) / scl.x;
   float zMod = (z - position().z) / scl.z;
   
   zMod += 1.0;
   
   return zMod * scl.y;
}