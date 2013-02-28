//
//  GameRamp.cpp
//  
//
//  Created by Bryan Bell on 1/30/13.
//
//

#include <stdlib.h>
#include <ctime>

#include "GameBuilding.h"
#include "PhongShader.h"

#define NUM_MATERIALS 3

PhongMaterial building_mats[NUM_MATERIALS] = {
   {vec3(0.2, 0.1, 0.0), // amb
      vec3(0.9, 0.5, 0.0), // diff
      vec3(0.1, 0.1, 0.1),       // spec
      20.0},               // shine
   
   {vec3(0.0, 0.2, 0.0), // amb
      vec3(0.0, 0.6, 0.0), // diff
      vec3(0.1, 0.1, 0.1),       // spec
      20.0},
   
   {vec3(0.2, 0.0, 0.2), // amb
      vec3(0.6, 0.4, 0.8), // diff
      vec3(0.1, 0.1, 0.1),       // spec
      20.0}
};

GameBuilding::GameBuilding() : GameSceneryObject("models/building.obj")
{
   int index = rand() % NUM_MATERIALS;
   
   for (int i = 0; i < meshStorage.numMeshes; i++) {
      if (meshStorage.material[i].dColor.x == 1.0f) {
         meshStorage.material[i] = building_mats[index];
      }
   }
}

float GameBuilding::getHeightAt(float x, float z)
{
   return getPosition().y + getScale().y;
}
