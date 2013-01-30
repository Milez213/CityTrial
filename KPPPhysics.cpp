//
//  KPPPhysics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "KPPPhysics.h"

KPPPhysics::Actor::Actor(glm::vec3 position, glm::vec3 velocity) :
   position(position), velocity(velocity) {}

void KPPPhysics::Actor::update(float dt)
{
   position += velocity * dt;
}

glm::vec3 KPPPhysics::Actor::get_position() {return position;}
glm::vec3 KPPPhysics::Actor::get_velocity() {return velocity;}
void KPPPhysics::Actor::set_position(glm::vec3 pos) {position = pos;}
void KPPPhysics::Actor::set_velocity(glm::vec3 vel) {velocity = vel;}

KPPPhysics::Actor *KPPPhysics::makeActor(glm::vec3 position, glm::vec3 direction)
{
   actors.push_back(Actor(position, direction));
   return &actors.back();
}

void KPPPhysics::simulate(double dt)
{
   for (std::list<Actor>::iterator itr = actors.begin(); itr != actors.end(); itr++)
      itr->update(dt);
}