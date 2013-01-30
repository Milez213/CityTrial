//
//  GameRamp.h
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#ifndef ____GameRamp__
#define ____GameRamp__

#include <iostream>

#include "GameDrawableObject.h"

class GameRamp : public GameDrawableObject {
public:
   GameRamp();
   
   float getHeightAt(float x, float z);
}

#endif /* defined(____GameRamp__) */
