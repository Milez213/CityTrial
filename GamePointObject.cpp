//
//  GamePointObject.cpp
//  
//
//  Created by Bryan Bell on 3/10/13.
//
//

#include "GamePointObject.h"

GamePointObject::GamePointObject(int pointValue) : GameDrawableObject("models/squash.obj") {
   points = pointValue;
   state = STAND;
   time = ((float)(rand() % 100) / 100.0f) * 3.0f;
}

void GamePointObject::onCollide(GameKartObject *other)
{
   if (state == STAND) {
      aim = other;
      state = PICKUP;
      vec3 movement = normalize(other->getVelocity());
      pos = other->getPosition() + movement * other->getRadius() * 3.0f;
      vel = movement * 20.0f;
      vel.y = 5.0f;
      time = 0.0;
   }
}

void GamePointObject::update(float dt)
{
   time += dt;
   if (state == STAND) {
      rot.y += 180.0f * dt;
      if (rot.y > 360.0f)
         rot.y -= 360.0f;
      pos.y = sin(time * 5.0) * 0.3 + 1.0;
   } else if (state == PICKUP || state == CATCH) {
      if (time > 0.5f)
         state = CATCH;
      vec3 newAim = normalize(aim->getPosition() - getPosition()) * time;
      vel += newAim;
      if (time >= 1.0f)
         vel = newAim * (aim->getTopSpeed() + 1.0f);
      if (pos.y < 0.5f) {
         pos.y = 0.5f;
         vel.y = fabs(vel.y * 0.5f);
      }
      GameObject::update(dt);
   }
}