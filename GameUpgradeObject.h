//
//  GameUpgradeObject.h
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#ifndef ____GameUpgradeObject__
#define ____GameUpgradeObject__

#include <iostream>

#include "GamePhysics.h"
#include "GameDrawableObject.h"

class GameUpgradeObject : public GameDrawableObject
{
public:
   enum Type {FLIGHT, GRIP, TURNING};
   
   GameUpgradeObject(Type initType, glm::vec3 position);
   
   void update(double time, double dt);
   
   Type upgradeType() { return type; };
   
private:
   
   static GamePhysicsActor *makeUpgradeActor(glm::vec3 position);
   Type type;
};
#endif /* defined(____GameUpgradeObject__) */
