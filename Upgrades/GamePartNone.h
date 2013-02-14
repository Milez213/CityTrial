//
//  GamePartNone.h
//  
//
//  Created by Eric Johnson on 2/11/13.
//
//

#ifndef _GamePartNone_h
#define _GamePartNone_h

#include "../GamePartUpgrade.h"

class GamePartNone : public GamePartUpgrade
{
   virtual void drawOnKart(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   virtual void addToKart(GameKartObject *kart) { assert(false); }
   virtual void cycleStatOn(GameKartProperties *props) {}
   virtual void cycleStatOff(GameKartProperties *props) {}
};

#endif
