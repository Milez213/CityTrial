
#ifndef GAME_KART_H
#define GAME_KART_H

#include "defines.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <stdarg.h>

#include "GamePhysicalObject.h"
#include "GameKartProperties.h"
#include "GamePartUpgrade.h"
#include "GameStatUpgrade.h"

#include "SoundManager.h"

// global variables
using std::vector;
using glm::translate;
using glm::scale;
using glm::rotate;


class GameKartObject : public GamePhysicalObject {
   
public:
   GameKartObject(const char *fileName);
   ~GameKartObject();

   virtual void update(float dt);
   
   virtual void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
   virtual void onCollide(GameDrawableObject *collide);
   
   virtual float getLift() {return properties.hasWings() ? 10.0/25 : 0;}
   float getRideHeight() { return getScale().y; }
   
   
   void addPart(GamePartUpgrade *part) { parts.push_back(part); }
   void addStat(GameStatUpgrade *stat) { stat->apply(&properties); }
   

   bool isUsingController() {
       return usingController;
   }

   void setUsingController(bool cont) {
       usingController = cont;
   }
   
   void setJoystickState(float joyState[]) { memcpy(joystickState, joyState, sizeof(float) * 4); }; //Allow main to set state of joysticks to do proper updating
   //void setButtonState(char butState[]) { memcpy(butState, buttonState, sizeof(char) * 32); }; //"  " of buttons to "  "
    
   //void stop();
   //void done();
   
   
private:
   PhongShader *meshShader;
   GameKartProperties properties;
   
   vector<GameDrawableObject *> wheels;
   vector<GamePartUpgrade *> parts;
   
   static const float tireTurnAngleTime;
   float tireAngle, tireTurnAngle;
   
   bool usingController,wings;
   float joystickState[4];
   char buttonState[32];

   GameSound *ding_sound;
   GameSound *collide_sound;
   
   void changeTireTurnAngle(float dt, float targetAngle);
};

#endif
