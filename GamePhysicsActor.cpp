//
//  GamePhysicsActor.cpp
//  
//
//  Created by Eric Johnson on 1/29/13.
//
//

#include <iostream>
#include "GamePhysicsActor.h"

#include "GameUtilities.h"
#include "foundation/PxTransform.h"

float GamePhysicsActor::speed()
{
   std::cerr << "gettingSpeed\n";
   return glm::length(velocity());
}
void GamePhysicsActor::setSpeed(float s)
{
   std::cerr << "setSpeedBegin\n";
   setVelocity(direction() * s);
   std::cerr << "setSpeedEnd\n";
}
glm::vec3 GamePhysicsActor::direction()
{
   std::cerr << "gettingDirection\n";
   return glm::normalize(velocity());
}
void GamePhysicsActor::setDirection(glm::vec3 vel)
{
   std::cerr << "setDirectionBegin\n";
   setVelocity(glm::normalize(vel) * speed());
   std::cerr << "setDirectionEnd\n";
}

glm::vec3 GamePhysicsActor::position() {
   return convert(mActor->getGlobalPose().p);
}
void GamePhysicsActor::setPosition(glm::vec3 pos)
{
   physx::PxTransform trans = mActor->getGlobalPose();
   trans.p = convert(pos);
   mActor->setGlobalPose(trans);
}
glm::vec3 GamePhysicsActor::velocity()
{
   std::cerr << "velocity start\n";
   physx::PxRigidBody *body = static_cast<physx::PxRigidBody *>(mActor);
   std::cerr << "velocity cast\n";
   physx::PxVec3 vel = body->getLinearVelocity();
   std::cerr << "velocity get\n";
   glm::vec3 rtn = convert(vel);
   std::cerr << "velocity convert\n";
   
   return rtn;
}
void GamePhysicsActor::setVelocity(glm::vec3 spd)
{
   std::cerr << "setVelocityBegin\n";
   (static_cast<physx::PxRigidBody *>(mActor))->setLinearVelocity(convert(spd));
   std::cerr << "setVelocityEnd\n";
}
