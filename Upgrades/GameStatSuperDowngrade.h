#ifndef _GameStatSpeed_h
#define _GameStatSpeed_h

#include "../GameStatUpgrade.h"

class GameStatSpeed : public GameStatUpgrade
{
   virtual void applyStat(GameKartProperties *props) { props->downgradeTopSpeed();
   props->downgradeAcceleration();
   props->downgradeMaxEnergy();
   props->downgradeEnergyRegen();
   props->downgradeBrakeSpeed();
 props->downgradeTurnSpeed();}
};

#endif
