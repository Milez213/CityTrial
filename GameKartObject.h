
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
    
   void stop();
   void done();
   
private:
   FlatShader *meshShader;
   
   vector<GameDrawableObject *> wheels;
};

#endif