//
//  GameActiveNone.h
//  
//
//  Created by Eric Johnson on 2/11/13.
//
//

#ifndef _GameActiveNone_h
#define _GameActiveNone_h

class GameActiveNone : public GameActiveUpgrade
{
public:
   virtual void drawEffect(PhongShader *meshShader, RenderingHelper modelViewMatrix) {}
   
   virtual void activeStart(GameKartObject *kart) {}
   virtual void activeUpdate(GameKartObject *kart, float dt) {}
   virtual void activeEnd(GameKartObject *kart) {}
};

#endif
