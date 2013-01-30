//
//  GamePhysics.h
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

using namespace glm;

struct bound {
   vec3 boundingBoxMin;
   vec3 boundingBoxMax;
   vec3 center;
   float radius;
};

class GamePhysics
{
public:
   class Actor
   {
   public:
      /*const glm::vec3 &getPosition();
      const glm::vec3 &getDirection();
      const bound &getBoundingInfo(); // Implement these or let me know how you'd like to do this
      void setBoundingInfo(bound boundingInfo);  //  See GameDrawableObject to see how I use it
   protected:
      Actor(glm::vec3 position, glm::vec3 direction);
      void update();*/
      
   private:
      glm::vec3 position;
      glm::vec3 direction;
      glm::vec3 boundingInfo;
   };
   
   void simulate();
   Actor *makeActor(glm::vec3 position, glm::vec3 direction);
   
private:
   std::list<Actor> actors;
};

#endif /* defined(____Physics__) */
