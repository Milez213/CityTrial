//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameTerrain.h"

GameTerrain::GameTerrain() : GameSceneryObject("floor")
{
   
}

float GameTerrain::getHeightAt(float x, float z)
{
   return getPosition().y;
}
float GameTerrain::getBottomAt(float x, float z)
{
   return getHeightAt(x, z);
}