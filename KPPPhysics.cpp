//
//  KPPPhysics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "KPPPhysics.h"

KPPPhysics::Actor(glm::vec3 position, glm::vec3 direction) :
   position(position), direction(direction) {}

void KPPPhysics::Actor::update()
{
   position += direction;
}

KPPPhysics::Actor *KPPPhysics::makeActor(const glm::vec3 &position, const glm::vec3 &direction)
{
   actors.push_back(Actor(position, direction));
   return &actors.back();
}

void KPPPhysics::simulate()
{
   for (std::list<Actor>::iterator itr = actors.begin(); itr != actors.end(); itr++)
      itr->update();
}