
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
   
   virtual void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix, float levelOfDetail);
   virtual void onCollide(GameDrawableObject *collide, float dt);
   
   virtual float getLift() { return properties.getLift(); } //{return properties.hasWings() ? 10.0/25 : 0;}
   float getRideHeight() { return getScale().y; }
   
   float getTopSpeed() { return properties.getTopSpeed(); }
   
   
   void addFrontPart(GamePartUpgrade *part) {frontPart = part; frontPartAdding = 1; frontScale = 0.99; frontScaleChanging = true; }
   void addSidePart(GamePartUpgrade *part) {sidePart = part; sidePartAdding = 1; sideScale = 0.99; sideScaleChanging = true;}
   void addBackPart(GamePartUpgrade *part) {backPart = part; backPartAdding = 1; backScale = 0.99; backScaleChanging = true; }
   
   void cycleFrontParts() { cyclePartList(frontParts); }
   void cycleSideParts() { cyclePartList(sideParts); }
   void cycleBackParts() { cyclePartList(backParts); }
   
   void addActive(GameActiveUpgrade *active);
   void cycleActives();
   
   float getPerspective() { return perspective; };
   

   bool isUsingController() {
       return usingController;
   }
   void setUsingController(bool cont) {
       usingController = cont;
   }
   
   void setJoystickState(float joyState[]) { memcpy(joystickState, joyState, sizeof(float) * 4); }; //Allow main to set state of joysticks to do proper updating
   float getJoystickState(int axis) { return joystickState[axis]; }
   void setButtonState(unsigned char butState[]) { memcpy(buttonState, butState, sizeof(unsigned char) * 32); }; //"  " of buttons to "  "
   
   void setInputMap(int up, int down, int left, int right,
      int action, int cycleActive, int cycleFront, int cycleSide, int cycleBack) {
      input.up = up; input.down = down; input.left = left; input.right = right;
      input.action = action; input.cycleActive = cycleActive;
      input.cycleFront = cycleFront; input.cycleSide = cycleSide; input.cycleBack = cycleBack;};
   int getInput(int request);
   const inputMap getInputMap() { return input; }
   
   void resize(float width, float height) { hud->setScreen(width, height); };
   void setHUDColor(vec3 color);
   void drawHUD(float dt, bool rushed);
   
   int getPoints() { return points; };
   float getEnergy() { return properties.getEnergy(); }
   float getMaxEnergy() { return properties.getMaxEnergy(); }
   
   bool isAirborn() { return airborn; }//return speed*getLift() > gravity; }
   
   void win();
   void lose();
   void reset();
    
   //void stop();
   //void done();
   void changeKartRollAngle(float dt,float rollAngle);
   void changeKartPitchAngle(float dt,float pitchAngle);
   
   void setSpawnPos(glm::vec3 pos) { spawnPos = pos; }
   virtual glm::vec3 getSpawnPos() { return spawnPos; }
   
protected:
   virtual void transform(RenderingHelper &modelViewMatrix);
   
private:
   PhongShader *meshShader;
   
   vector<GameDrawableObject *> wheels;
   list<GamePartUpgrade *> frontParts;
   list<GamePartUpgrade *> sideParts;
   list<GamePartUpgrade *> backParts;
   list<GameActiveUpgrade *> activeUpgrades;
   
   static const float tireTurnAngleTime;
   float tireAngle, tireTurnAngle, carPitchAngle, carRollAngle;
   float frontScale, backScale, sideScale;
   bool frontScaleChanging, backScaleChanging, sideScaleChanging;
   int frontScaleDir, backScaleDir, sideScaleDir, frontPartAdding, sidePartAdding, backPartAdding;
   
   GamePartUpgrade *frontPart, *sidePart, *backPart;

   glm::vec3 spawnPos;
   
   bool airborn;
   
   bool usingController;//,wings;
   float joystickState[4];
   unsigned char buttonState[32];
   bool actionOn;
   bool buttonDown[32];
   
   inputMap input;
   
   int points;
   int winState;
   
   float perspective;

   GameSound *ding_sound;
   GameSound *collide_sound;
   GameSound *activate_part_sound;
   GameSound *deactivate_part_sound;
   GameSound *flying_sound;
   GameSound *outOfEnergy_sound;
   // to only play sound once per state change
   bool playedFlyingSound;
   bool pausedFlyingSound;
   bool playedOutOfEnergy; 
   
   GameHUD *hud;
   
   void changeTireTurnAngle(float dt, float mult, float speedDampedTurnAngle);
   void addPartToList(list<GamePartUpgrade *> &list, GamePartUpgrade *part);
   void cyclePartList(list<GamePartUpgrade *> &list);
   void changePartScale(int part,int dir, float dt);
};

#endif
