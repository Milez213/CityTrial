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

class GamePhysics : physx::PxSimulationEventCallback
{
public:
   GamePhysics();
   
   void simulate(double dt);
   void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
   //ignores these callbacks but has to define them not pure
   void onConstraintBreak (physx::PxConstraintInfo *constraints, physx::PxU32 count) {}
   void onWake (physx::PxActor **actors, physx::PxU32 count) {}
   void onSleep (physx::PxActor **actors, physx::PxU32 count) {}
   void onTrigger (physx::PxTriggerPair *pairs, physx::PxU32 count) {}
   
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
