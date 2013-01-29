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

#include "GameObject.h"
#include "FlatShader.h"

#include "MStackHelp.h"

using namespace glm;

class GameDrawableObject : public GameObject {
public:
   GameDrawableObject(const char *objFile);
   
   void draw(FlatShader *meshShader, RenderingHelper modelViewMatrix);
    
protected:
   GLuint vertexArray;
   GLuint vertexBuffer, textureBuffer, normalBuffer;
   GLuint *indexBuffer;
   int *indexBufferLength;
};

#endif