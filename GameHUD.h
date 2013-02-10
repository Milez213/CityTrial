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
#include "PhongShader.h"

#include "GameObject.h"

using namespace glm;

class GameHUD {
public:
   GameHUD(float width, float height);
   
   void drawSpeed(PhongShader *meshShader, RenderingHelper modelViewMatrix, float speed);
   
protected:
   bufferStore meshStorage;
   
   float hudWidth, hudHeight;
};


#endif /* defined(____GameHUD__) */
