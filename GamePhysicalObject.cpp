//
//  GamePhysicalObject.cpp
//  
//
//  Created by Eric Johnson on 2/2/13.
//
//

#include "GamePhysicalObject.h"


glm::vec3 GamePhysicalObject::getVelocity()
{
   vec3 vel = getSpeed() * getDirectionVector();
   vel.y = -fallSpeed;
   return vel;
}

void GamePhysicalObject::update(float dt)
{
   fallSpeed += gravity*dt - speed*getLift()*dt;
   
   if (getPosition().y < scl.y && fallSpeed > 0) {
      fallSpeed = 0;
   }
   
   /*vec3 oldPos = getPosition();
   float newHeight = oldPos.y - gravity*dt;
   
   if (newHeight < scl.y)
      newHeight = scl.y;
   
   setPosition(vec3(oldPos.x, newHeight, oldPos.z));*/
   
   GameDrawableObject::update(dt);
}
void GamePhysicalObject::onCollide(GameDrawableObject *other)
{
   cout << "collision!\n";
   if (dynamic_cast<GamePhysicalObject *>(other) != NULL) setSpeed(0);
}