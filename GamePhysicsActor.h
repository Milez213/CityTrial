//
//  GamePhysicsActor.h
//  
//
//  Created by Eric Johnson on 1/29/13.
//
//

#ifndef ____GamePhysicsActor__
#define ____GamePhysicsActor__

#include <iostream>
#include "glm/glm.hpp"
#include "PxRigidActor.h"

/*using namespace glm;

struct bound {
   vec3 boundingBoxMin;
   vec3 boundingBoxMax;
   vec3 center;
   float radius;
};*/

class GamePhysicsActor
{
   friend class GamePhysics;
   
public:
   void link(void *obj) { mActor->userData = obj; }
   
   
   void push(float force);
   
   glm::vec3 direction();
	void setDirection(glm::vec3 d);
   //float direction();
	//void setDirection(float d);
	float speed();
	void setSpeed(float s);
   
   glm::vec3 velocity();
	void setVelocity(glm::vec3 v);
	glm::vec3 position();
	void setPosition(glm::vec3 p);
   
protected:
   GamePhysicsActor(physx::PxRigidActor *actor) : mActor(actor) {lastDir = glm::vec3(0,0,1); lastSpeed = 0;}
   
   void addForce(glm::vec3 f);
   void clearForce();
   
private:
   physx::PxRigidActor *mActor;
   glm::vec3 lastDir;
   float lastSpeed;
   
};

#endif /* defined(____GamePhysicsActor__) */
