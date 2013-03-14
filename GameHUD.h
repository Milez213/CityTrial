//
//  GameHUD.h
//  
//
//  Created by Bryan Bell on 2/9/13.
//
//

#ifndef ____GameHUD__
#define ____GameHUD__

#include <iostream>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#include "MStackHelp.h"
#include "ModelManager.h"
#include "HUDShader.h"

using namespace glm;

class GameHUD {
public:
   GameHUD();
   
   void prepareShader();
   void setColor(vec3 color) { playerColor = color; };
   
   void setScreen(float width, float height);
   void drawWin();
   void drawLose();
   void drawSpeed(float speed);
   void drawEnergy(float maxEnergy, float energy, string name);
   void drawScore(int score);
   void drawTimer(float dt);
   
protected:
   RenderingHelper modelMatrix;
   
   mat4 proj, view;
   
   float hudWidth, hudHeight;
   
private:
   void setOrthographicMatrix();
   void setHUDView();
   
   float currentSpeed;
   float currentTime;
   
   int currentScore;
   
   vec3 playerColor;
};


#endif /* defined(____GameHUD__) */
