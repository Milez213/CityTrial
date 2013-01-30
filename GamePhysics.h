//
//  GamePhysics.h
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#ifndef ____GamePhysics__
#define ____GamePhysics__

#include <iostream>
#include <list>
#include "GamePhysicsActor.h"
#include "PxPhysicsAPI.h"

class GamePhysics
{
public:
   GamePhysics();
   void simulate();
   GamePhysicsActor *makeActor(glm::vec3 position, glm::vec3 direction);
   
private:
   std::list<GamePhysicsActor> actors;
};

#endif /* defined(____GamePhysics__) */
