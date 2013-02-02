//
//  GameSceneObject.h
//  
//
//  Created by Eric Johnson on 2/2/13.
//
//

#ifndef ____GameSceneObject__
#define ____GameSceneObject__

#include <iostream>

#include "GamePhysicalObject.h"

class GameSceneryObject : public GamePhysicalObject {
public:
   GameSceneryObject(const char *objFile) : GamePhysicalObject(objFile) {};
   
   virtual float getHeightAt(float x, float z) = 0;
};


#endif /* defined(____GameSceneObject__) */
