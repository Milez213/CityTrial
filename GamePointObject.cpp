//
//  GamePointObject.cpp
//  
//
//  Created by Bryan Bell on 3/10/13.
//
//

#include "GamePointObject.h"

#define NUM_MATERIALS 3

float point_diff[NUM_MATERIALS * 3] = {
   0.0, 1.0, 0.0,
   1.0, 0.0, 0.0,
   0.0, 0.0, 1.0
};

float point_spec[NUM_MATERIALS * 3] = {
   0.0, 1.0, 0.0,
   1.0, 0.0, 0.0,
   0.0, 0.0, 1.0
};

GamePointObject::GamePointObject(int pointValue) : GameDrawableObject("models/squash.obj") {
   points = pointValue;
   state = STAND;
   time = ((float)(rand() % 100) / 100.0f) * 3.0f;
   
   int index = rand() % NUM_MATERIALS;
   int index2 = rand() % NUM_MATERIALS;
   
   for (unsigned int LoD = 0; LoD < meshStorage.size(); LoD++) {
      for (int i = 0; i < meshStorage[LoD].numMeshes; i++) {
         meshStorage[LoD].material[i].dColor = vec3(point_diff[index*3], point_diff[index*3+1],
                                                    point_diff[index*3+2]);
         meshStorage[LoD].material[i].sColor = vec3(point_spec[index2*3], point_spec[index2*3+1],
                                                    point_spec[index2*3+2]);
      }
   }
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
   vec3 rotate = getRotation();
   vec3 position = getPosition();
   vec3 velocity = getVelocity();
   time += dt;
   if (state == STAND) {
      rotate.y += 180.0f * dt;
      if (rotate.y > 360.0f)
         rotate.y -= 360.0f;
      position.y = sin(time * 5.0) * 0.3 + 1.0 + basePosition.y;
   } else if (state == PICKUP || state == CATCH) {
      if (time > 0.5f)
         state = CATCH;
      vec3 newAim = normalize(aim->getPosition() - getPosition()) * time;
      velocity += newAim;
      if (time >= 1.0f)
         velocity = newAim * (aim->getTopSpeed() + 1.0f);
      if (position.y < 0.5f) {
         position.y = 0.5f;
         velocity.y = fabs(velocity.y * 0.5f);
      }
      
      if (time < 1.0f) {
         setScale(vec3((5.0f-time * 4.0f)/10.0f, (5.0f-time*4.0f)/10.0f, (5.0f-time*4.0f)/10.0f));
      } else {
         setScale(vec3(1.0f/10.0f, 1.0f/10.0f, 1.0f/10.0f));
      }
   }
   
   setRotation(rotate);
   setPosition(position);
   setVelocity(velocity);
   
   GameObject::update(dt);
}