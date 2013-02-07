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
   GameStatUpgrade() : GameUpgradeObject("cube") {}
};

#endif /* defined(____GameStatUpgrade__) */
