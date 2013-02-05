//
//  GameSceneObject.h
//  
//
//  Created by Eric Johnson on 2/2/13.
//
//

#ifndef ____GameSceneryObject__
#define ____GameSceneryObject__

#include <iostream>

#include "GameDrawableObject.h"

class GameSceneryObject : public GameDrawableObject {
public:
   GameSceneryObject(const char *objFile) : GameDrawableObject(objFile) {};
   
   virtual void onCollide(GameDrawableObject *other) {}
   
   virtual float getHeightAt(float x, float z) = 0;
   virtual float getBottomAt(float x, float z) { return getPosition().y-getScale().y; }
};


#endif /* defined(____GameSceneryObject__) */
