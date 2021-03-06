//
//  GameActiveNone.h
//  
//
//  Created by Eric Johnson on 2/11/13.
//
//

#ifndef _GameActiveNone_h
#define _GameActiveNone_h

#include "../GameActiveUpgrade.h"

class GameActiveNone : public GameActiveUpgrade
{
public:
   GameActiveNone() : GameActiveUpgrade("none") {}
   
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   virtual bool activeStart(GameKartObject *kart) {return false;}
   virtual bool activeUpdate(GameKartObject *kart, float dt) {assert(false);}
   virtual void activeEnd(GameKartObject *kart) {}
};

#endif
