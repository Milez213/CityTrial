//
//  GamePhysicsActor.cpp
//  
//
//  Created by Eric Johnson on 1/29/13.
//
//

#include "GamePhysicsActor.h"

#include "GameUtilities.h"
#include "foundation/PxTransform.h"

glm::vec3 GamePhysicsActor::position() {
   return convert(mActor->getGlobalPose().p);
}
void GamePhysicsActor::setPosition(glm::vec3 pos)
{
   mActor->setGlobalPose();
}
glm::vec3 GamePhysicsActor::direction() {return glm::vec3();}
void GamePhysicsActor::setDirection(glm::vec3 vel) {}
float GamePhysicsActor::speed() {return 0;}
void GamePhysicsActor::setSpeed(float spd) {}
