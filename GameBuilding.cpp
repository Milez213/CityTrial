//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameBuilding.h"

GameBuilding::GameBuilding() : GameSceneryObject("cube")
{
   
}

float GameBuilding::getHeightAt(float x, float z)
{
   return scl.y*2;
}