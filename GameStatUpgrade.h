//
//  GameStatUpgrade.h
//  
//
//  Created by Eric Johnson on 2/6/13.
//
//

#ifndef ____GameStatUpgrade__
#define ____GameStatUpgrade__

#include <iostream>
#include "GameUpgradeObject.h"

class GameStatUpgrade : public GameUpgradeObject
{
public:
   GameStatUpgrade(const char *name) : GameUpgradeObject(name) {}
   GameStatUpgrade() : GameUpgradeObject("cube") {}
   
   virtual void addToKart(GameKartObject *kart) { applyStat( &kart->properties); }
   virtual void applyStat(GameKartProperties *props) = 0;
};

#endif /* defined(____GameStatUpgrade__) */
