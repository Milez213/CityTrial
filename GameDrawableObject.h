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

#include "include_gl.h"

#include "MStackHelp.h"
#include "ModelManager.h"
#include "PhongShader.h"

#include "GameObject.h"

using namespace glm;

class GameDrawableObject : public GameObject {
public:
   GameDrawableObject(const char *objFile);
   
   virtual void draw(PhongShader *meshShader, RenderingHelper modelViewMatrix);
   //virtual void draw(PhongShader *meshShader);
   //virtual void drawSpecial(PhongShader *meshShader, RenderingHelper modelViewMatrix,float pitchAngle, float rollAngle);
   
   virtual void onCollide(GameDrawableObject *other);

   virtual void setScale(vec3 s);
   virtual void setPosition(vec3 p);
   
   void scheduleForRemoval() { toRemove = true; };
   bool isScheduledForRemoval() { return toRemove; };
   
   float getRadius() { return boundingInfo.radius; };
   const char* getName() { return meshStorage.name.c_str(); };
   
   void updateBoundingInfo();
   const bound getBoundingInfo();

   virtual void setMaterialIndex(int i) {
       materialIndex = i;
   }
   virtual int getMaterialIndex() {
       return materialIndex;
   }
    
protected:
   static const unsigned short LOD_COUNT = 1;
   
   virtual void transform(RenderingHelper &modelViewMatrix);
   
   bufferStore meshStorage[LOD_COUNT];
   bound boundingInfo;
   
   bool toRemove;

   int materialIndex;
};

#endif
