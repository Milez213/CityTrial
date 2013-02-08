
#ifndef GAME_KART_H
#define GAME_KART_H

#include "defines.h"

#include <iostream>
#include <vector>
#include <list>
#include <cstring>
#include <stdlib.h>
#include <stdarg.h>

#include "GamePhysicalObject.h"
#include "GameKartProperties.h"

#include "SoundManager.h"

struct inputMap {
   int up, down, left, right;
};

// global variables
using std::vector;
using glm::translate;
using glm::scale;
using glm::rotate;

class GamePartUpgrade;
class GameStatUpgrade;

class GameKartObject : public GamePhysicalObject {
   
public:
   GameKartProperties properties;
   
   
   GameKartObject(const char *fileName);
   ~GameKartObject();

   virtual void update(float dt);
   
   virtual void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
   virtual void onCollide(GameDrawableObject *collide);
   
   virtual float getLift() {return properties.hasWings() ? 10.0/25 : 0;}
   float getRideHeight() { return getScale().y; }
   
   
   void addFrontPart(GamePartUpgrade *part) { frontParts.push_front(part); }
   void addSidePart(GamePartUpgrade *part) { sideParts.push_front(part); }
   void addBackPart(GamePartUpgrade *part) { backParts.push_front(part); }
   

   bool isUsingController() {
       return usingController;
   }

   void setUsingController(bool cont) {
       usingController = cont;
   }
   
   void setJoystickState(float joyState[]) { memcpy(joystickState, joyState, sizeof(float) * 4); }; //Allow main to set state of joysticks to do proper updating
   //void setButtonState(char butState[]) { memcpy(butState, buttonState, sizeof(char) * 32); }; //"  " of buttons to "  "
   
   void setInputMap(int up, int down, int left, int right) { input.up = up; input.down = down; input.left = left; input.right = right; };
   int getInput(int request);
   
    
   //void stop();
   //void done();
   
   
private:
   PhongShader *meshShader;
   
   vector<GameDrawableObject *> wheels;
   list<GamePartUpgrade *> frontParts;
   list<GamePartUpgrade *> sideParts;
   list<GamePartUpgrade *> backParts;
   
   static const float tireTurnAngleTime;
   float tireAngle, tireTurnAngle;
   
   bool usingController,wings;
   float joystickState[4];
   char buttonState[32];
   
   inputMap input;

   GameSound *ding_sound;
   GameSound *collide_sound;
   
   void changeTireTurnAngle(float dt, float mult, float speedDampedTurnAngle);
};

#endif
