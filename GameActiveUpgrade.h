//
//  GameActiveUpgrade.h
//  
//
//  Created by Eric Johnson on 2/10/13.
//
//

#ifndef ____GameActiveUpgrade__
#define ____GameActiveUpgrade__

#include <iostream>
#include "GameUpgradeObject.h"

class GameActiveUpgrade : GameUpgradeObject
{
   GameActiveUpgrade() : GameUpgradeObject("ramp") {}
   
   void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) = 0;
   void activeOnce() = 0;
   void activeDrain() = 0;
};

#endif /* defined(____GameActiveUpgrade__) */
