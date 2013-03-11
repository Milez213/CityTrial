//
//  GamePointObject.h
//  
//
//  Created by Bryan Bell on 3/10/13.
//
//

#ifndef ____GamePointObject__
#define ____GamePointObject__

#include <iostream>

#include "GameKartObject.h"
#include "GameDrawableObject.h"

#define STAND 0
#define PICKUP 1
#define CATCH 2

class GamePointObject : public GameDrawableObject {
public:
   GamePointObject(int pointValue);
   
   virtual void update(float dt);
   virtual void onCollide(GameKartObject *other);
   
   int getPoints() { return points; };
   int getState() { return state; };
   
   bool pickUp() {
      if (state == CATCH)
         return true;
      else
         return false; };
   
private:
   int points;
   int state;
   
   float time;
   
   GameKartObject *aim;
};

#endif /* defined(____GamePointObject__) */
