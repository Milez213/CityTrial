#ifndef _GameStatSpeed_h
#define _GameStatSpeed_h

#include "../GameStatUpgrade.h"

class GameStatSpeed : public GameStatUpgrade
{
   virtual void applyStat(GameKartProperties *props) { props->upgradeTopSpeed();
   props->upgradeAcceleration();
   props->upgradeMaxEnergy();
   props->upgradeEnergyRegen();
   props->upgradeBrakeSpeed();
 props->upgradeTurnSpeed();}
};

#endif
