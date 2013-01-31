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
   void push(float force);
   
   glm::vec3 direction();
	void setDirection(glm::vec3 d);
   glm::vec3 velocity();
	void setVelocity(glm::vec3 v);
	float speed();
	void setSpeed(float s);
	glm::vec3 position();
	void setPosition(glm::vec3 p);
   
protected:
   GamePhysicsActor(physx::PxRigidActor *actor) : mActor(actor) {}
   //void update();
   
   void addForce(glm::vec3 f);
   void clearForce();
   
private:
   physx::PxRigidActor *mActor;
   
};

#endif /* defined(____GamePhysicsActor__) */
