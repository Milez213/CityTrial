//
//  GamePhysicsActor.cpp
//  
//
//  Created by Eric Johnson on 1/29/13.
//
//

#include "GamePhysicsActor.h"


GamePhysicsActor::GamePhysicsActor(glm::vec3 position, glm::vec3 velocity) :
   position(position), velocity(velocity) {}

void GamePhysicsActor::update()
{
   position += velocity;
}

glm::vec3 GamePhysicsActor::get_position() {return position;}
glm::vec3 GamePhysicsActor::get_velocity() {return velocity;}
void GamePhysicsActor::set_position(glm::vec3 pos) {position = pos;}
void GamePhysicsActor::set_velocity(glm::vec3 vel) {velocity = vel;}
