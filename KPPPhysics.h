//
//  KPPPhysics.h
//  
//
//  Created by Eric Johnson on 1/25/13.
//
//

#ifndef ____Physics__
#define ____Physics__

#include <iostream>
#include <list>
#include "glm/glm.hpp"

class KPPPhysics
{
public:
   class Actor
   {
      friend class KPPPhysics;
      
   public:
      glm::vec3 get_position();
      glm::vec3 get_velocity();
      void set_position(glm::vec3 pos);
      void set_velocity(glm::vec3 vel);
   protected:
      Actor(glm::vec3 position, glm::vec3 velocity);
      void update(float dt);
      
   private:
      glm::vec3 position;
      glm::vec3 velocity;
   };
   
   void simulate(double dt);
   Actor *makeActor(glm::vec3 position, glm::vec3 direction);
   
private:
   std::list<Actor> actors;
};

#endif /* defined(____Physics__) */
