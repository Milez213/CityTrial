
#ifndef GAME_KART_H
#define GAME_KART_H

#include "defines.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdarg.h>

#include "FlatShader.h"
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
   void draw(FlatShader *meshShader, RenderingHelper modelViewMatrix);

   bool collide(GamePhysicsActor *collide);
   
   bool usingController() { return usingController };
   void setUsingController(bool cont) { usingController = cont; };
   
   //void setJoystickState(float joyState[]) { memCpy(joyState, joystickState, sizeof(float) * 4); }; //Allow main to set state of joysticks to do proper updating
   //void setButtonState(char butState[]) { memCpy(butState, buttonState, sizeof(char) * 32); }; //"  " of buttons to "  "
    
   void stop();
   void done();
   
private:
   FlatShader *meshShader;
   
   vector<GameDrawableObject *> wheels;
   
   bool usingController;
   float joystickState[4];
   char buttonState[32];
};

#endif