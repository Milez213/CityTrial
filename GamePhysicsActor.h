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

using namespace glm;

struct bound {
   vec3 boundingBoxMin;
   vec3 boundingBoxMax;
   vec3 center;
   float radius;
};

class GamePhysicsActor
{
   friend class GamePhysics;
   
public:
   glm::vec3 get_position();
   glm::vec3 get_velocity();
   void set_position(glm::vec3 pos);
   void set_velocity(glm::vec3 vel);
   
protected:
   GamePhysicsActor(glm::vec3 position, glm::vec3 velocity);
   void update();
   
private:
   glm::vec3 position;
   glm::vec3 velocity;
   glm::vec3 boundingInfo;
};

#endif /* defined(____GamePhysicsActor__) */
