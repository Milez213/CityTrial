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
#include "GameKartObject.h"

class GameUpgradeObject : public GameDrawableObject
{
public:
   //enum Type {FLIGHT, GRIP, TURNING, SPEED};
   
   //GameUpgradeObject(Type initType);
   GameUpgradeObject(const char *objFile);
   
   virtual void update(float dt);
   virtual void onCollide(GameDrawableObject *other);
   
   virtual void addToKart(GameKartObject *kart) = 0;
   virtual void applyStat(GameKartProperties *props) = 0;
   
   //Type upgradeType() { return type; };
   
private:
   //Type type;
};
#endif /* defined(____GameUpgradeObject__) */
