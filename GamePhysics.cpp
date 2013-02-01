//
//  GamePhysics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "GamePhysics.h"
#include "GameObject.h"

using namespace physx;

void fatalError(std::string message)
{
   std::cerr << message << std::endl;
   exit ( EXIT_FAILURE );
}

GamePhysics::GamePhysics()
{
   static physx::PxDefaultErrorCallback gDefaultErrorCallback;
   static physx::PxDefaultAllocator gDefaultAllocatorCallback;
   
   mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
   if(!mFoundation)
      fatalError("PxCreateFoundation failed!");
   
   bool recordMemoryAllocations = true;
   mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
   if(!mProfileZoneManager)
      fatalError("PxProfileZoneManager::createProfileZoneManager failed!");
   
   mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
                              physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager );
   if(!mPhysics)
      fatalError("PxCreatePhysics failed!");
   
   physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
   sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
   //customizeSceneDesc(sceneDesc);
   
   if(!sceneDesc.cpuDispatcher)
   {
      mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(mNbThreads);
      if(!mCpuDispatcher)
         fatalError("PxDefaultCpuDispatcherCreate failed!");
      sceneDesc.cpuDispatcher    = mCpuDispatcher;
   }
   if(!sceneDesc.filterShader)
      sceneDesc.filterShader    = &physx::PxDefaultSimulationFilterShader;
   
   #ifdef PX_WINDOWS
   if(!sceneDesc.gpuDispatcher && mCudaContextManager)
   {
      sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
   }
   #endif
   
   mScene = mPhysics->createScene(sceneDesc);
   if (!mScene)
      fatalError("createScene failed!");
   
   
   std::cout << "PhysX successfully initialized\n";
}


GamePhysicsActor *GamePhysics::makeActor(physx::PxRigidActor *actor)
{
   mScene->addActor(*actor);
   actors.push_back(GamePhysicsActor(actor));
   return &actors.back();
}

physx::PxRigidDynamic *GamePhysics::makeBlankDynamic(physx::PxTransform pose)
{
   return mPhysics->createRigidDynamic(pose);
}

GamePhysicsActor *GamePhysics::makeStaticActor(physx::PxTransform pose, physx::PxGeometry *geom, physx::PxMaterial *mat)
{
   physx::PxRigidStatic *actor = PxCreateStatic(*mPhysics, pose, *geom, *mat);
   
   return makeActor(actor);
}
GamePhysicsActor *GamePhysics::makeDynamicActor(physx::PxTransform pose, physx::PxGeometry *geom, physx::PxMaterial *mat, double mass)
{
   physx::PxRigidDynamic *actor = makeBlankDynamic(pose);
   
   actor->createShape(*geom, *mat);
   actor->setMass(mass);
   
   //physx::PxRigidDynamic *actor = PxCreateDynamic(*mPhysics, pose, *geom, *mat, density);
   
   return makeActor(actor);
}




void GamePhysics::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
   for(PxU32 i=0; i < nbPairs; i++)
   {
      const PxContactPair& cp = pairs[i];
      
      if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
      {
         GameObject *objs[2];
         objs[0] = reinterpret_cast<GameObject *>(pairHeader.actors[0]->userData);
         objs[1] = reinterpret_cast<GameObject *>(pairHeader.actors[1]->userData);
         
         objs[0]->collide(objs[1]);
         objs[1]->collide(objs[0]);
         
         /*if((pairHeader.actors[0] == mSubmarineActor) || (pairHeader.actors[1] == mSubmarineActor))
         {
            PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ?
            pairHeader.actors[1] : pairHeader.actors[0];
            Seamine* mine =  reinterpret_cast<Seamine*>(otherActor->userData);
            // insert only once
            if(std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
               mMinesToExplode.push_back(mine);
            
            break;
         }*/
      }
   }
}

void GamePhysics::simulate(double dt)
{
   mScene->simulate(dt);
   mScene->fetchResults(true);
}