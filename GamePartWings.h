//
//  GamePartWings.h
//  
//
//  Created by Eric Johnson on 2/6/13.
//
//

#ifndef _GamePartWings_h
#define _GamePartWings_h

#include "GamePartUpgrade.h"

class GamePartWings : public GamePartUpgrade
{
   virtual void drawOnKart(PhongShader *meshShader, RenderingHelper modelViewMatrix)
   {
      
   }
   virtual void apply(GameKartProperties *props) { props->setWings(true); }
   virtual void unApply(GameKartProperties *props) { props->setWings(false); }
};


#endif
