//
//  Physics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "Physics.h"

Physics::Actor(glm::vec3 position, glm::vec3 direction) :
   position(position), direction(direction) {}

void Physics::Actor::update()
{
   position += direction;
}

Physics::Actor *Physics::makeActor(const glm::vec3 &position, const glm::vec3 &direction)
{
   actors.push_back(Actor(position, direction));
   return &actors.back();
}

void Physics::simulate()
{
   for (std::list<Actor>::iterator itr = actors.begin(); itr != actors.end(); itr++)
      itr->update();
}