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
   bool hasJetEngine() {return jetEngine;}
   void toggleJetEngine() {jetEngine = !jetEngine;}
   void setJetEngine(bool on) {jetEngine = on;};*/
   
   //stats
   static const float MAX_ENERGY_CHANGE = 10.0;
   float getMaxEnergy() {return maxEnergy;}
   void upgradeMaxEnergy() {maxEnergy += MAX_ENERGY_CHANGE;}
   void downgradeMaxEnergy() {maxEnergy -= MAX_ENERGY_CHANGE;}
   
   static const float ENERGY_REGEN_CHANGE = 5.0;
   float getEnergyRegen() {return energyRegen;}
   void upgradeEnergyRegen() {energyRegen += ENERGY_REGEN_CHANGE;}
   void downgradeEnergyRegen() {energyRegen -= ENERGY_REGEN_CHANGE;}
   
   static const float ACCELERATION_CHANGE = 2.5;
   float getAcceleration() {return acceleration;}
   void upgradeAcceleration(int num = 1) {acceleration += ACCELERATION_CHANGE*num;}
   void downgradeAcceleration(int num = 1) {acceleration -= ACCELERATION_CHANGE*num;}
   
   static const float TOP_SPEED_CHANGE = 5.0;
   float getTopSpeed() {return topSpeed;}
   void upgradeTopSpeed(int num = 1) {topSpeed += TOP_SPEED_CHANGE*num;}
   void downgradeTopSpeed(int num = 1) {topSpeed -= TOP_SPEED_CHANGE*num;}
   
   static const float BRAKE_SPEED_CHANGE = 5.0;
   float getBrakeSpeed() {return brakeSpeed;}
   void upgradeBrakeSpeed(int num = 1) {brakeSpeed += BRAKE_SPEED_CHANGE*num;}
   void downgradeBrakeSpeed(int num = 1) {brakeSpeed -= BRAKE_SPEED_CHANGE*num;}
   
   static const float TURN_SPEED_CHANGE = 5.0;
   float getTurnSpeed() {return turnSpeed;}
   void upgradeTurnSpeed(int num = 1) {turnSpeed += TURN_SPEED_CHANGE*num;}
   void downgradeTurnSpeed(int num = 1) {turnSpeed -= TURN_SPEED_CHANGE*num;}
   
   float getLift() {return lift;}
   void setLift(float lift) {this->lift = lift;}
   //void upgradeLift() {lift += 5;}
   
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
   bool useEnergyCost(float cost) {
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
