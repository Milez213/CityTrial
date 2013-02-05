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
   fallSpeed += 2*gravity*dt;
   fallSpeed -= 2*speed*getLift()*dt;
   
   if (getPosition().y < -10) {
      setPosition(vec3(0, 5, 0));
      speed = 0;
      fallSpeed = 0;
   }
   
   /*if (getPosition().y < scl.y && fallSpeed > 0) {
      fallSpeed = 0;
   }*/
   
   GameDrawableObject::update(dt); //updates position
}
void GamePhysicalObject::onCollide(GameDrawableObject *other, float dt)
{
   cout << "collision!\n";
   if (dynamic_cast<GamePhysicalObject *>(other) != NULL) setSpeed(0);
}