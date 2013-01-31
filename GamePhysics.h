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
#include "glm/glm.hpp"
#include "PxPhysicsAPI.h"
#include "GamePhysicsActor.h"

class GamePhysics
{
public:
   GamePhysics();
   void simulate(double dt);
   GamePhysicsActor *makeActor(physx::PxRigidActor *actor);
   physx::PxRigidDynamic *makeBlankDynamic(physx::PxTransform pose);
   GamePhysicsActor *makeStaticActor(physx::PxTransform pose, physx::PxGeometry *geom, physx::PxMaterial *mat);
   GamePhysicsActor *makeDynamicActor(physx::PxTransform pose, physx::PxGeometry *geom, physx::PxMaterial *mat, double mass);
   physx::PxMaterial *makeMaterial() { return mPhysics->createMaterial(0.5f, 0.5f, 0.1f); }
   
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
