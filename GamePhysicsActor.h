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
   glm::vec3 get_position();
   glm::vec3 get_velocity();
   void set_position(glm::vec3 pos);
   void set_velocity(glm::vec3 vel);
   
protected:
   GamePhysicsActor(physx::PxRigidActor *actor) : mActor(actor) {}
   //void update();
   
private:
   physx::PxRigidActor *mActor;
   
};

#endif /* defined(____GamePhysicsActor__) */
