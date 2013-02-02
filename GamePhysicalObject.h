//
//  GamePhysicalObject.h
//  
//
//  Created by Eric Johnson on 2/2/13.
//
//

#ifndef ____GamePhysicalObject__
#define ____GamePhysicalObject__

#include <iostream>
#include "GameDrawableObject.h"

class GamePhysicalObject : public GameDrawableObject
{
public:
   static float gravity;
   float lift;
   float dir;
   float spd;
   
   virtual glm::vec3 getVelocity() { return glm::vec3(spd * cos(dir), 0, spd * sin(dir)); };
   virtual float getDirection() { return dir; };
	virtual void setDirection(float d) { dir = d; };
	virtual float getSpeed() { return spd; };
	virtual void setSpeed(float s) { spd = s; }
   
   void onCollide(GameObject *other) { if (dynamic_cast<GamePhysicalObject>(other) != NULL) speed = 0; };
   
   GamePhysicalObject(const char *objFile) : GameDrawableObject(objFile) {}
};


#endif /* defined(____GamePhysicalObject__) */
