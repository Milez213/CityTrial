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
#include <string>

#include "GameDrawableObject.h"
#include "GameKartObject.h"

#include "SoundManager.h"

class GameUpgradeObject : public GameDrawableObject
{
public:
   //enum Type {FLIGHT, GRIP, TURNING, SPEED};
   
   //GameUpgradeObject(Type initType);
   GameUpgradeObject(const char *objFile);
   
   virtual void update(float dt);
   virtual void onCollide(GameDrawableObject *other, float dt);
   
   virtual void addToKart(GameKartObject *kart) = 0;

   virtual void playPickupSound();
   
   //Type upgradeType() { return type; };
   
   string getName() { return upgradeName; };
   void setName(string name) { upgradeName = name; };
   
private:
   //Type type;

   GameSound *pickup_sound;
   
   string upgradeName;
   
};
#endif /* defined(____GameUpgradeObject__) */
