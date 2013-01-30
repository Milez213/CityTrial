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
   GamePhysicsActor *makeStaticActor(physx::PxTransform pose, physx::PxGeometry geom, physx::PxMaterial *mat);
   GamePhysicsActor *makeDynamicActor(physx::PxTransform pose, physx::PxGeometry geom, physx::PxMaterial *mat, double density);
   
private:
   physx::PxFoundation *mFoundation;
   physx::PxProfileZoneManager *mProfileZoneManager;
   physx::PxPhysics *mPhysics;
   physx::PxScene *mScene;
   physx::pxtask::CpuDispatcher *mCpuDispatcher;
   static const int mNbThreads = 1;
   
   std::list<GamePhysicsActor> actors;
};

#endif /* defined(____GamePhysics__) */
