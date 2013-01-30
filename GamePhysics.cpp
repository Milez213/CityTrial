//
//  GamePhysics.cpp
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#include "GamePhysics.h"

GamePhysics::GamePhysics()
{
   
}

GamePhysicsActor *GamePhysics::makeActor(glm::vec3 position, glm::vec3 direction)
{
   actors.push_back(GamePhysicsActor(position, direction));
   return &actors.back();
}

void GamePhysics::simulate()
{
   for (std::list<GamePhysicsActor>::iterator itr = actors.begin(); itr != actors.end(); itr++)
      itr->update();
}