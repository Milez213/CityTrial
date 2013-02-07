//
//  GamePartUpgrade.h
//  
//
//  Created by Eric Johnson on 2/6/13.
//
//

#ifndef ____GamePartUpgrade__
#define ____GamePartUpgrade__

#include <iostream>
#include "GameUpgradeObject.h"

class GamePartUpgrade : public GameUpgradeObject
{
public:
   GamePartUpgrade() : GameUpgradeObject("ramp") {}
   
   virtual void drawOnKart(PhongShader *meshShader, RenderingHelper modelViewMatrix) = 0;
   virtual void unApply(GameKartProperties *props) = 0;
};

#endif /* defined(____GamePartUpgrade__) */
