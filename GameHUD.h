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
   
   void setScreen(float width, float height);
   void drawSpeed(float speed);
   
protected:
   RenderingHelper modelMatrix;
   
   mat4 proj, view;
   
   float hudWidth, hudHeight;
   
private:
   void setOrthographicMatrix();
   void setHUDView();
};


#endif /* defined(____GameHUD__) */
