//
//  GameKartProperties.h
//  
//
//  Created by Eric Johnson on 2/2/13.
//
//

#ifndef ____GameKartProperties__
#define ____GameKartProperties__

#include <iostream>

class GameKartProperties
{
public:
   GameKartProperties() : /*wings(false), jetEngine(false),*/ maxEnergy(100), energyRegen(10),
   acceleration(10), topSpeed(25), brakeSpeed(20), turnSpeed(45), lift(0), energy(maxEnergy) {}
   
   /*//parts
   bool hasWings() {return wings;}
   void toggleWings() {wings = !wings;}
   void setWings(bool on) {wings = on;};
   
   bool hasJetEngine() {return jetEngine;}
   void toggleJetEngine() {jetEngine = !jetEngine;}
   void setJetEngine(bool on) {jetEngine = on;};*/
   
   //stats
   float getMaxEnergy() {return maxEnergy;}
   void upgradeMaxEnergy() {maxEnergy += 10;}
   float getEnergyRegen() {return energyRegen;}
   void upgradeEnergyRegen() {energyRegen += 5;}
   float getAcceleration() {return acceleration;}
   void upgradeAcceleration() {acceleration += 1;}
   float getTopSpeed() {return topSpeed;}// + (hasJetEngine() ? 10 : 0);}
   void upgradeTopSpeed() {topSpeed += 5;}
   float getBrakeSpeed() {return brakeSpeed;}
   void upgradeBrakeSpeed() {brakeSpeed += 5;}
   float getTurnSpeed() {return turnSpeed;}
   void upgradeTurnSpeed() {turnSpeed += 5;}
   float getLift() {return lift;}
   void setLift(float lift) {this->lift = lift;}
   void upgradeLift() {lift += 5;}
   
   //consumables
   void regenEnergy(float dt) {
      energy += dt*getEnergyRegen();
      if (energy > getMaxEnergy())
         energy = getMaxEnergy();
   }
   bool useEnergyDrain(float drainSpeed, float dt) {
      if (energy - dt*drainSpeed > 0) {
         energy -= dt*drainSpeed;
         return true;
      }
      else {
         return false;
      }
   }
   bool useEnergyOnce(float cost) {
      if (energy - cost > 0) {
         energy -= cost;
         return true;
      }
      else {
         return false;
      }
   }
   float getEnergy() { return energy; }
   
private:
   /*//parts
   bool wings;
   bool jetEngine;*/
   
   //stats
   float maxEnergy;
   float energyRegen;
   float acceleration;
   float topSpeed;
   float brakeSpeed;
   float turnSpeed;
   float lift;
   
   //consumables
   float energy;
};

#endif /* defined(____GameKartProperties__) */
