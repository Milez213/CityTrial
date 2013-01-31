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
   return glm::normalize(velocity());
}
void GamePhysicsActor::setDirection(glm::vec3 vel)
{
   setVelocity(glm::normalize(vel) * speed());
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
   return convert((dynamic_cast<physx::PxRigidBody *>(mActor))->getLinearVelocity());
}
void GamePhysicsActor::setVelocity(glm::vec3 spd)
{
   (dynamic_cast<physx::PxRigidBody *>(mActor))->setLinearVelocity(convert(spd));
}
