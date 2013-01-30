//
//  GamePhysics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "GamePhysics.h"

/*GamePhysics::Actor(glm::vec3 position, glm::vec3 direction)
{
   position(position);
   direction(direction);
}*/

/*void GamePhysics::Actor::update()
{
   position += direction;
}*/

GamePhysics::Actor *GamePhysics::makeActor(const glm::vec3 position, const glm::vec3 direction)
{
   /*actors.push_back(Actor(position, direction));
   return &actors.back();*/
   
   return NULL;
}

void GamePhysics::simulate()
{
   for (std::list<Actor>::iterator itr = actors.begin(); itr != actors.end(); itr++) ;
      //itr->update();
}