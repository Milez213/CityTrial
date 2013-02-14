//
//  GameStatAcceleration.h
//  
//
//  Created by Eric Johnson on 2/13/13.
//
//

#ifndef _GameStatAcceleration_h
#define _GameStatAcceleration_h

#include "../GameStatUpgrade.h"

class GameStatAcceleration : public GameStatUpgrade
{
   virtual void applyStat(GameKartProperties *props) { props->upgradeAcceleration(); }
};

#endif
