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
   GamePartUpgrade() : GameUpgradeObject("models/ball.obj") {}
   GamePartUpgrade(const char *file) : GameUpgradeObject(file) {}
   
   virtual void drawOnKart(PhongShader *meshShader, RenderingHelper modelViewMatrix, float levelOfDetail) = 0;
   virtual void cycleStatOn(GameKartProperties *props) = 0;
   virtual void cycleStatOff(GameKartProperties *props) = 0;
};

#endif /* defined(____GamePartUpgrade__) */
