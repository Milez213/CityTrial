//
//  GameSceneObject.h
//  
//
//  Created by Eric Johnson on 2/2/13.
//
//

#ifndef ____GameSceneryObject__
#define ____GameSceneryObject__

#include <iostream>

#include "GameDrawableObject.h"

class GameSceneryObject : public GameDrawableObject {
public:
   GameSceneryObject(const char *objFile) : GameDrawableObject(objFile) {};
   
   virtual void onCollide(GameDrawableObject *other) {}
   
   virtual float getHeightAt(float x, float z) = 0;
   virtual float getBottomAt(float x, float z) { return getPosition().y-getScale().y; }
   virtual glm::vec3 getTopVectorAt(float x, float z) { return glm::vec3(0,1,0); }
   virtual glm::vec3 getSideVectorAt(float x, float z) {
      glm::vec3 pos = getPosition();
      glm::vec3 scl = getScale();
      
      if (pos.x + scl.x - x < x - (pos.x - scl.x)) {
         if (pos.z + scl.z - z < z - (pos.z - scl.z)) {
            if (pos.x + scl.x - x > pos.z + scl.z - z) {
               return glm::vec3(1, 0, 0);
            }
            else {
               return glm::vec3(0, 0, 1);
            }
         }
         else {
            if (pos.x + scl.x - x > z - (pos.z - scl.z)) {
               return glm::vec3(1, 0, 0);
            }
            else {
               return glm::vec3(0, 0, -1);
            }
         }
      }
      else {
         if (pos.z + scl.z - z < z - (pos.z - scl.z)) {
            if (x - (pos.x - scl.x) > pos.z + scl.z - z) {
               return glm::vec3(-1, 0, 0);
            }
            else {
               return glm::vec3(0, 0, 1);
            }
         }
         else {
            if (x - (pos.x - scl.x) > z - (pos.z - scl.z)) {
               return glm::vec3(-1, 0, 0);
            }
            else {
               return glm::vec3(0, 0, -1);
            }
         }
      }
   }
};


#endif /* defined(____GameSceneryObject__) */
