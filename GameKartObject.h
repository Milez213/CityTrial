
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
#include "GameHUD.h"

#include "SoundManager.h"

struct inputMap {
   int up, down, left, right, action, cycleActive, cycleFront, cycleSide, cycleBack;
};

// global variables
using std::vector;
using glm::translate;
using glm::scale;
using glm::rotate;

//class GameStatUpgrade;
class GamePartUpgrade;
class GameActiveUpgrade;



class GameKartObject : public GamePhysicalObject {
public:
   GameKartProperties properties;
   
   
   GameKartObject(const char *fileName);
   ~GameKartObject();

   virtual void update(float dt);
   
   virtual void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
   virtual void onCollide(GameDrawableObject *collide);
   
   virtual float getLift() { return properties.getLift(); } //{return properties.hasWings() ? 10.0/25 : 0;}
   float getRideHeight() { return getScale().y; }
   
   
   void addFrontPart(GamePartUpgrade *part) { addPartToList(frontParts, part); }
   void addSidePart(GamePartUpgrade *part) { addPartToList(sideParts, part); }
   void addBackPart(GamePartUpgrade *part) { addPartToList(backParts, part); }
   
   void cycleFrontParts() { cyclePartList(frontParts); }
   void cycleSideParts() { cyclePartList(sideParts); }
   void cycleBackParts() { cyclePartList(backParts); }
   
   void addActive(GameActiveUpgrade *active);
   void cycleActives();
   

   bool isUsingController() {
       return usingController;
   }
   void setUsingController(bool cont) {
       usingController = cont;
   }
   
   void setJoystickState(float joyState[]) { memcpy(joystickState, joyState, sizeof(float) * 4); }; //Allow main to set state of joysticks to do proper updating
   void setButtonState(unsigned char butState[]) { memcpy(buttonState, butState, sizeof(unsigned char) * 32); }; //"  " of buttons to "  "
   
   void setInputMap(int up, int down, int left, int right,
      int action, int cycleActive, int cycleFront, int cycleSide, int cycleBack) {
      input.up = up; input.down = down; input.left = left; input.right = right;
      input.action = action; input.cycleActive = cycleActive;
      input.cycleFront = cycleFront; input.cycleSide = cycleSide; input.cycleBack = cycleBack;};
   int getInput(int request);
   const inputMap getInputMap() { return input; }
   
   void resize(float width, float height) { hud->setScreen(width, height); };
   void drawHUD();
   
   int getPoints() { return points; };
   float getEnergy() { return properties.getEnergy(); }
   float getMaxEnergy() { return properties.getMaxEnergy(); }
   
   void win();
   void lose();
    
   //void stop();
   //void done();
   
   
private:
   PhongShader *meshShader;
   
   vector<GameDrawableObject *> wheels;
   list<GamePartUpgrade *> frontParts;
   list<GamePartUpgrade *> sideParts;
   list<GamePartUpgrade *> backParts;
   list<GameActiveUpgrade *> activeUpgrades;
   
   static const float tireTurnAngleTime;
   float tireAngle, tireTurnAngle, carPitchAngle, carRollAngle;
   
   bool usingController;//,wings;
   float joystickState[4];
   unsigned char buttonState[32];
   bool actionOn;
   bool buttonDown[32];
   
   inputMap input;
   
   int points;

   GameSound *ding_sound;
   GameSound *collide_sound;
   GameSound *activate_part_sound;
   GameSound *deactivate_part_sound;
   GameSound *flying_sound;
   GameSound *outOfEnergy_sound;
   
   GameHUD *hud;
   
   void changeTireTurnAngle(float dt, float mult, float speedDampedTurnAngle);
   void changeKartRollAngle(float dt,float rollAngle);
   void changeKartPitchAngle(float dt,float pitchAngle);
   void addPartToList(list<GamePartUpgrade *> &list, GamePartUpgrade *part);
   void cyclePartList(list<GamePartUpgrade *> &list);
};

#endif
