/*
 * GameDrawableObject.h
 * Author: Chris Pauley & Bryan Bell
 *
 * Date: 1/28/2013
 * This class defines a base drawable game
 *  object. Its only unique attributes from
 *  "GameObject" are its buffer objects used
 *  to render the object.
 */

#ifndef GAME_DRAW_OBJECT
#define GAME_DRAW_OBJECT

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#include "MStackHelp.h"
#include "ModelManager.h"
#include "PhongShader.h"

#ifdef USE_PHYSX
#include "GamePhysics.h"
#endif

#include "GameObject.h"

#include "Mesh.h"

#include <stddef.h>


using namespace glm;

class GameDrawableObject : public GameObject {
public:
   GameDrawableObject(const char *objFile);
   
   void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
    
protected:
   bufferStore meshStorage;

   Mesh *mesh;
   
   //bound boundInfo;
   
   // GamePhysicsActor *actor;
};

#endif
