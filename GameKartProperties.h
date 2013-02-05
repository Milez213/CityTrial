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
   GameKartProperties() : wings(false), acceleration(10), topSpeed(25), brakeSpeed(20), turnSpeed(45) {}
   
   //parts
   bool hasWings() {return wings;}
   void toggleWings() {wings = !wings;}
   void setWings() {wings = true;};
   
   //stats
   float getAcceleration() {return acceleration;}
   void upgradeAcceleration() {acceleration += 1;}
   float getTopSpeed() {return topSpeed;}
   void upgradeTopSpeed() {topSpeed += 5;}
   float getBrakeSpeed() {return brakeSpeed;}
   void upgradeBrakeSpeed() {brakeSpeed += 5;}
   float getTurnSpeed() {return turnSpeed;}
   void upgradeTurnSpeed() {turnSpeed += 5;}
   
private:
   //parts
   bool wings;
   
   //stats
   float acceleration;
   float topSpeed;
   float brakeSpeed;
   float turnSpeed;
};

#endif /* defined(____GameKartProperties__) */
