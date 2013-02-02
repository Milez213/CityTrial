
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

// global variables
using std::vector;
using glm::translate;
using glm::scale;
using glm::rotate;


class GameKartObject : public GamePhysicalObject {
   
public:
   GameKartObject(const char *fileName);
   ~GameKartObject();

   void update(float dt);
   void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);

   void collide(GameObject *collide);
   
   void changeTireTurnAngle(float targetAngle);

   bool isUsingController() {
       return usingController;
   }

   void setUsingController(bool cont) {
       usingController = cont;
   }
   
   void setJoystickState(float joyState[]) { memcpy(joystickState, joyState, sizeof(float) * 4); }; //Allow main to set state of joysticks to do proper updating
   //void setButtonState(char butState[]) { memcpy(butState, buttonState, sizeof(char) * 32); }; //"  " of buttons to "  "
    
   void stop();
   void done();
   
private:
   PhongShader *meshShader;
   GamePartProperties properties;
   
   vector<GameDrawableObject *> wheels;
   vector <GameDrawableObject *> upgrades;
   
   float tireAngle, tireTurnAngle;
   
   bool usingController,wings;
   float joystickState[4];
   char buttonState[32];
};

#endif
