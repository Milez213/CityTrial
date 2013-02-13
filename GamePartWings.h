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

#include "SoundManager.h"

extern SoundManager *g_sound_manager;


class GamePartWings : public GamePartUpgrade {

public:
   GamePartWings() : GamePartUpgrade() {
   }

   virtual void drawOnKart(PhongShader *meshShader, RenderingHelper modelViewMatrix)
   {
      modelViewMatrix.pushMatrix();
      modelViewMatrix.scale(0.1,0.1,5.0);
      modelViewMatrix.rotate(80.0,vec3(0.0,0.0,1.0));
      GameDrawableObject::draw(meshShader,modelViewMatrix);
      modelViewMatrix.popMatrix();
   }
   virtual void addToKart(GameKartObject *kart)
   {
      setPosition(glm::vec3(0.0));
      setScale(glm::vec3(1.0));
      setRotation(glm::vec3(0.0));
      kart->addSidePart(this);
      //cycleStatOn(&kart->properties);
   }
   virtual void cycleStatOn(GameKartProperties *props) {
       activate_sound = g_sound_manager->getSample("sounds/wings_select.wav");
       activate_sound->play();
       props->setLift(props->getLift()+10.0/25);
   }
   virtual void cycleStatOff(GameKartProperties *props) { props->setLift(props->getLift()-10.0/25); }

private:
   GameSound *activate_sound;
};


#endif
