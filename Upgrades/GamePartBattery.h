//
//  GamePartBattery.h
//  
//
//  Created by Eric Johnson on 2/13/13.
//
//

#ifndef _GamePartBattery_h
#define _GamePartBattery_h

#include "../GamePartUpgrade.h"

extern SoundManager *g_sound_manager;


class GamePartBattery : public GamePartUpgrade {
   
public:
   GamePartBattery() : GamePartUpgrade() {
      activate_sound = g_sound_manager->getSample("sounds/wings_select.wav");
   }
   
   virtual void drawOnKart(PhongShader *meshShader, RenderingHelper modelViewMatrix)
   {
      modelViewMatrix.pushMatrix();
      modelViewMatrix.scale(0.5, 1.75 , 1.5);
      modelViewMatrix.translate(glm::vec3(0.0, 0.25, 0.0));
      GameDrawableObject::draw(meshShader,modelViewMatrix);
      modelViewMatrix.popMatrix();
   }
   virtual void addToKart(GameKartObject *kart)
   {
      setPosition(glm::vec3(0.0));
      setScale(glm::vec3(1.0));
      setRotation(glm::vec3(0.0));
      kart->addBackPart(this);
      //cycleStatOn(&kart->properties);
   }
   virtual void cycleStatOn(GameKartProperties *props) {
      activate_sound->play();
      props->upgradeMaxEnergy(3);
      props->upgradeEnergyRegen(3);
   }
   virtual void cycleStatOff(GameKartProperties *props)
   {
      props->downgradeMaxEnergy(3);
      props->downgradeEnergyRegen(3);
   }
   
   virtual void playPickupSound() {
      // overwrite default generic_pickup.ogg in GameUpgradeObject
      // pickup sound is activate sound
      // Parts activate when they're picked up.
   }
   
private:
   GameSound *activate_sound;
   
};

#endif
