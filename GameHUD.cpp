//
//  GameHUD.cpp
//  
//
//  Created by Bryan Bell on 2/9/13.
//
//

#include "GameHUD.h"

extern HUDShader *hudShader;

GameHUD::GameHUD()
{   
   modelMatrix.useModelViewMatrix();
   modelMatrix.loadIdentity();
   
   setOrthographicMatrix();
   setHUDView();
   
#ifdef DEBUG_VBO
   //printf("VBO Arrived at its Destination: %d\n", (int)indexBufferLength[0]);
#endif
}

void GameHUD::setOrthographicMatrix() {
   proj = glm::ortho(0.0f, (float)hudWidth, (float)hudHeight, 0.0f, -1.0f, 1.0f);
}

void GameHUD::setHUDView() {
   view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 1.0f ),glm::vec3( 0.0f, 0.0f, 0.0f ),glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void GameHUD::setScreen(float width, float height)
{
   hudWidth = width;
   hudHeight = height;

   setOrthographicMatrix();
}

#define SPD 75.0
void GameHUD::drawSpeed(float speed)
{
   hudShader->use();
   
   // this is messy.
   // We should use x and y offsets in Screen coordinates, etc
   // and pass them to the shader
   // Then we won't need any matrices, like in TTFRenderer.h --Mustafa
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(hudWidth - SPD, hudHeight - SPD, 0.0));
   modelMatrix.scale(SPD + speed, SPD, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   hudShader->setViewMatrix(view);
   hudShader->setProjectionMatrix(proj);

   hudShader->draw();

   hudShader->deactivate();
}
