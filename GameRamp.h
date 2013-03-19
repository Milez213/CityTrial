//
//  GameRamp.h
//  
//
//  Created by Bryan Bell on 1/30/13.
//
// NOTE: Do not rotate this object by anything but the y-axis,
//  it will not work properly if the object is not flat.

#ifndef ____GameRamp__
#define ____GameRamp__

#include <iostream>

#include "GameSceneryObject.h"

class GameRamp : public GameSceneryObject {
public:
   GameRamp();
   
   virtual float getHeightAt(float x, float z);
   virtual float getBottomAt(float x, float z) { return getHeightAt(x, z) - getScale().y/5; }
   virtual glm::vec3 getTopVectorAt(float x, float z);
};

#endif /* defined(____GameRamp__) */
