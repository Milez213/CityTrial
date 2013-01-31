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
   glm::vec3 direction();
	void setDirection(glm::vec3 d);
	float speed();
	void setSpeed(float s);
	glm::vec3 position();
	void setPosition(glm::vec3 p);
   
protected:
   GamePhysicsActor(physx::PxRigidActor *actor) : mActor(actor) {}
   //void update();
   
private:
   physx::PxRigidActor *mActor;
   
};

#endif /* defined(____GamePhysicsActor__) */
