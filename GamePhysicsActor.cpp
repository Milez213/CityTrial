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


void GamePhysicsActor::push(float force)
{
   clearForce();
   addForce(force * direction());
}

float GamePhysicsActor::speed()
{
   return lastSpeed;
   /*glm::vec3 vel = velocity();
   vel.y = 0;
   return glm::length(vel);*/
   //return glm::length(velocity());
}
void GamePhysicsActor::setSpeed(float s)
{
   lastSpeed = s;
   glm::vec3 vel = velocity();
   glm::vec3 dir = direction();
   
   vel.x = s * dir.x;
   vel.z = s * dir.z;
   
   setVelocity(vel);
   //setVelocity(direction() * s);
}
/*float GamePhysicsActor::direction()
{
   return dir;
}
void GamePhysicsActor::setDirection(float dir)
{
   this->dir = dir;
}*/
glm::vec3 GamePhysicsActor::direction()
{
   
   glm::vec3 vel = velocity();
   vel.y = 0;
   
   /*if (vel.x || vel.z)
      return glm::normalize(vel);
   else*/
      return lastDir;
   //return glm::normalize(velocity());
}
void GamePhysicsActor::setDirection(glm::vec3 d)
{
   glm::vec3 vel = velocity();
   float s = speed();
   glm::vec3 dir = d;
   d.y = 0;
   
   //if (dir.x || dir.z) {
      //dir = glm::normalize(d);
      lastDir = dir;

      vel.x = s * dir.x;
      vel.z = s * dir.z;
   
      setVelocity(vel);
   //}
   
      
   //setVelocity(glm::normalize(dir) * speed());
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
   return convert(static_cast<physx::PxRigidDynamic *>(mActor)->getLinearVelocity());;
}
void GamePhysicsActor::setVelocity(glm::vec3 vel)
{
   std::cerr << "setVelocityBegin\n";
   (static_cast<physx::PxRigidDynamic *>(mActor))->setLinearVelocity(convert(vel));
   std::cerr << "setVelocityEnd\n";
}

void GamePhysicsActor::addForce(glm::vec3 force)
{
   (static_cast<physx::PxRigidBody *>(mActor))->addForce(convert(force));
}
void GamePhysicsActor::clearForce()
{
   (static_cast<physx::PxRigidBody *>(mActor))->clearForce();
}