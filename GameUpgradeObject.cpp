//
//  GameUpgradeObject.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include "GameUpgradeObject.h"
#include "GameKartObject.h"


/*GameUpgradeObject::GameUpgradeObject(Type initType) : GameDrawableObject("cube")
{
   type = initType;
   name = "upgrade";
   toRemove = false;
}*/
GameUpgradeObject::GameUpgradeObject(const char *objFile) : GameDrawableObject(objFile)
{
   
}

void GameUpgradeObject::update(float dt)
{
   //setPosition(vec3(getPosition().x, getPosition().y + 0.01 * cos(time), getPosition().z));
   
   float yRot = getRotation().y + 90 * dt;
   if (yRot > 360) 
      yRot -= 360;

   setRotation(vec3(getRotation().x, yRot, getPosition().z));
}

void GameUpgradeObject::onCollide(GameDrawableObject *other)
{
   if (GameKartObject *kart =  dynamic_cast<GameKartObject *>(other)) {
      /*switch (type) {
         case SPEED:
            kart->properties.upgradeSPeed();
            break;
            
         case WINGS:
            kart->addPart(new GamePartWingObject())
         default:
            break;
      }*/
      scheduleForRemoval();
   }
}
