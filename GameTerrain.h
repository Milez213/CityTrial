//
//  GameRamp.h
//  
//
//  Created by Bryan Bell on 1/30/13.
//
// NOTE: Do not rotate this object by anything but the y-axis,
//  it will not work properly if the object is not flat.

#ifndef ____GameTerrain__
#define ____GameTerrain__

#include <iostream>

#include "GameSceneryObject.h"

class GameTerrain : public GameSceneryObject {
public:
   GameTerrain();
   
   float getHeightAt(float x, float z);
   float getBottomAt(float x, float z);
};

#endif /* defined(____GameTerrain__) */
