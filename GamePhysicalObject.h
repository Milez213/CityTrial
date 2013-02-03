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
   float direction;
   float speed;
   float fallSpeed;
   //float lift;
   
   virtual float getDirection() { return TO_DEGREES(-direction); };
	virtual void setDirection(float dir) { direction = TO_RADIANS(-dir); };
	virtual float getSpeed() { return speed; };
	virtual void setSpeed(float spd) { speed = spd; }
   virtual float getLift() { return 0; }
   
   virtual glm::vec3 getDirectionVector() { return glm::vec3(cos(direction), 0, sin(direction)); };
   virtual glm::vec3 getVelocity();
   
   virtual void update(float dt);
   virtual void onCollide(GameObject *other);
   
   GamePhysicalObject(const char *objFile) : GameDrawableObject(objFile), direction(0), speed(0), fallSpeed(0) {} //, lift(0) {}
};


#endif /* defined(____GamePhysicalObject__) */
