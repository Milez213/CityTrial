
#ifndef GAME_KART_H
#define GAME_KART_H

#include "defines.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <stdarg.h>

#include "GamePhysics.h"
#include "GameDrawableObject.h"

// global variables
using std::vector;
using glm::translate;
using glm::scale;
using glm::rotate;


class GameKartObject : public GameDrawableObject {
   
public:
   GameKartObject(const char *fileName);
   ~GameKartObject();

   void update(double dt);
   void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);

   bool collide(GamePhysicsActor *collide);
   
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
   
   static GamePhysicsActor *makeKartActor();
   static GamePhysicsActor *makeTireActor();
   vector<GameDrawableObject *> wheels;
   vector <GameDrawableObject *> upgrades;
   
   float acceleration, topSpeed, turningRadius;
   
   bool usingController;
   float joystickState[4];
   char buttonState[32];
};

#endif
