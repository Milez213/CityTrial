//
//  GameStatSpeed.h
//  
//
//  Created by Eric Johnson on 2/6/13.
//
//

#ifndef _GameStatSpeed_h
#define _GameStatSpeed_h

#include "../GameStatUpgrade.h"

class GameStatSpeed : public GameStatUpgrade
{
public:
   GameStatSpeed() : GameStatUpgrade("models/speedometer.obj") {}
   
   virtual void applyStat(GameKartProperties *props) { props->upgradeTopSpeed(); }
};

#endif
