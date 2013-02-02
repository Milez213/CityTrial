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

#include "GameDrawableObject.h"

class GameUpgradeObject : public GameDrawableObject
{
public:
   enum Type {FLIGHT, GRIP, TURNING};
   
   GameUpgradeObject(Type initType);
   
   void update(double time, double dt);
   
   Type upgradeType() { return type; };
   
private:
   
   Type type;
};
#endif /* defined(____GameUpgradeObject__) */
