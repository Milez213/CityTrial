//
//  GamePhysics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "GamePhysics.h"

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

GamePhysicsActor *GamePhysics::makeStaticActor(physx::PxTransform pose, physx::PxGeometry geom, physx::PxMaterial *mat)
{
   physx::PxRigidStatic *actor = PxCreateStatic(*mPhysics, pose, geom, *mat);
   mScene->addActor(*actor);
   
   actors.push_back(GamePhysicsActor(actor));
   return &actors.back();
}
GamePhysicsActor *GamePhysics::makeDynamicActor(physx::PxTransform pose, physx::PxGeometry geom, physx::PxMaterial *mat, double density)
{
   physx::PxRigidDynamic *actor = PxCreateDynamic(*mPhysics, pose, geom, *mat, density);
   mScene->addActor(*actor);   
   
   actors.push_back(GamePhysicsActor(actor));
   return &actors.back();
}

void GamePhysics::simulate(double dt)
{
   mScene->simulate(dt);
   mScene->fetchResults(true);
}