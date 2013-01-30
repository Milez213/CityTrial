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
#include "PxPhysicsAPI.h"
#include "GamePhysicsActor.h"

class GamePhysics
{
public:
   GamePhysics();
   void simulate(double dt);
   GamePhysicsActor *makeActor(glm::vec3 position, glm::vec3 direction);
   
private:
   physx::PxFoundation *mFoundation;
   physx::PxProfileZoneManager *mProfileZoneManager;
   physx::PxPhysics *mPhysics;
   physx::PxScene *mScene;
   physx::pxtask::CpuDispatcher *mCpuDispatcher;
   static const int mNbThreads = 1;
   static const float mStepSize = 1.0f/60.0f;
   
   std::list<GamePhysicsActor> actors;
};

#endif /* defined(____GamePhysics__) */
