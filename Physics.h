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

#include "include_glm.h"



class KPPPhysics
{
public:
   class Actor
   {
   public:
      const glm::vec3 &getPosition();
      const glm::vec3 &getDirection();
   protected:
      Agent(glm::vec3 position, glm::vec3 direction);
      void update();
      
   private:
      glm::vec3 position;
      glm::vec3 direction;
   };
   
   void simulate();
   Actor *makeActor(glm::vec3 position, glm::vec3 direction);
   
private:
   std::list<Actor> actors;
};

#endif /* defined(____Physics__) */
