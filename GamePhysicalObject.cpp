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
   vel += this->vel;
   return vel;
}

void GamePhysicalObject::update(float dt)
{
   fallSpeed += 2*gravity*dt;
   fallSpeed -= 2*speed*getLift()*dt;
   
   if (getPosition().y < -10) {
      setPosition(getSpawnPos());
      speed = 0;
      fallSpeed = 0;
   }
   
   if (getPosition().y > 100) {
      vec3 pos = getPosition();
      setPosition(vec3(pos.x, 100, pos.z));
      fallSpeed = 0;
   }
   
   vec3 signs(sign(vel.x), sign(vel.y), sign(vel.z));
   vel.x -= signs.x * friction * dt;
   if (vel.x * signs.x < 0)
      vel.x = 0;
   vel.y -= signs.y* friction * dt;
   if (vel.y * signs.y < 0)
      vel.y = 0;
   vel.z -= signs.z* friction * dt;
   if (vel.z * signs.z < 0)
      vel.z = 0;
   
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