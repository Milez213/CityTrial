//
//  GameStatBrake.h
//  
//
//  Created by Eric Johnson on 2/13/13.
//
//

#ifndef _GameStatBrake_h
#define _GameStatBrake_h


#include "../GameStatUpgrade.h"

class GameStatBrake : public GameStatUpgrade
{
   virtual void applyStat(GameKartProperties *props) { props->upgradeBrakeSpeed(); }
};

#endif
