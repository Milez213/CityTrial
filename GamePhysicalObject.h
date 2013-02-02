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
#include "defines.h"

class GamePhysicalObject : public GameDrawableObject
{
public:
   static const float gravity = 9.81;
   static const float friction = 5;
   float lift;
   float direction;
   float speed;
   
   virtual float getDirection() { return TO_DEGREES(-dir); };
	virtual void setDirection(float d) { dir = TO_RADIANS(-d); };
	virtual float getSpeed() { return spd; };
	virtual void setSpeed(float s) { spd = s; }
   
   virtual glm::vec3 getDirectionVector() { return glm::vec3(cos(dir), 0, sin(dir)); };
   virtual glm::vec3 getVelocity() { return getSpeed() * getDirectionVector(); };
   
   virtual void update(float dt) { GameDrawableObject::update(dt); };
   virtual void onCollide(GameObject *other) { if (dynamic_cast<GamePhysicalObject *>(other) != NULL) setSpeed(0); };
   
   GamePhysicalObject(const char *objFile) : GameDrawableObject(objFile), lift(0), direction(0), speed(0) {}
};


#endif /* defined(____GamePhysicalObject__) */
