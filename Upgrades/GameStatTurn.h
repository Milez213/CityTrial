//
//  GameStatTurn.h
//  
//
//  Created by Eric Johnson on 2/13/13.
//
//

#ifndef _GameStatTurn_h
#define _GameStatTurn_h

#include "../GameStatUpgrade.h"

class GameStatTurn : public GameStatUpgrade
{
   virtual void applyStat(GameKartProperties *props) { props->upgradeTurnSpeed(); }
};

#endif
