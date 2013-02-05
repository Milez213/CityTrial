//
//  GameRamp.h
//  
//
//  Created by Bryan Bell on 1/30/13.
//
// NOTE: Do not rotate this object by anything but the y-axis,
//  it will not work properly if the object is not flat.

#ifndef ____GameBuilding__
#define ____GameBuilding__

#include <iostream>

#include "GameSceneryObject.h"

class GameBuilding : public GameSceneryObject {
public:
   GameBuilding();
   
   float getHeightAt(float x, float z);
};

#endif /* defined(____GameBuilding__) */
