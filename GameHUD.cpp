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
   
   currentSpeed = 0.0;
   currentScore = 0;
   currentTime = 0.0;
   
   playerColor = vec3(1.0, 0.5, 0.5);
   
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

void GameHUD::prepareShader()
{
   hudShader->use();
   
   hudShader->setViewMatrix(view);
   hudShader->setProjectionMatrix(proj);
   
   hudShader->setColor(playerColor);
}

void GameHUD::drawWin()
{
   float width, height;
   
   width = hudWidth/2.0;
   height = hudHeight/2.0;
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(width/2.0, height/2.0, 0.0));
   modelMatrix.scale(width, height, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->draw(string("win"));
}

void GameHUD::drawLose()
{
   float width, height;
   
   width = hudWidth/2.0;
   height = hudHeight/2.0;
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(width/2.0, height/2.0, 0.0));
   modelMatrix.scale(width, height, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->draw(string("lose"));
}

#define TIME_WDT 100.0f
#define TIME_HGT 100.0f
#define GROW 20.0f
void GameHUD::drawTimer(float dt, bool rushed)
{
   currentTime += dt;
   
   if (currentTime > 1.0f)
      currentTime -= 1.0f;
   
   float scl = GROW * currentTime;
   if (!rushed)
      scl = 0;
   float posX = hudWidth / 2.0f - (TIME_WDT + scl) / 2.0f;
   float posY = -(scl) / 2.0f;
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(posX, posY, 0.0));
   modelMatrix.scale(TIME_WDT + scl, TIME_HGT + scl, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->useTextureColor();
   
   hudShader->draw(string("timer"));
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(posX + TIME_WDT / 2.0f, posY + TIME_HGT / 2.0f, 0.0));
   modelMatrix.scale(TIME_WDT, TIME_HGT, 1.0);
   modelMatrix.rotate(360.0 * currentTime, vec3(0.0, 0.0, 1.0));
   modelMatrix.translate(vec3(-0.5, -0.5, 0.0));
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->draw(string("tneedle"));
   
   hudShader->deactivate();
}

#define SPD 150.0
void GameHUD::drawSpeed(float speed)
{
   if (speed - currentSpeed > 1.0)
      currentSpeed++;
   else if (currentSpeed - speed > 1.0)
      currentSpeed--;
   else
      currentSpeed = speed;
   
   if (currentSpeed < 0.0)
      currentSpeed = 0.0;
   
   float width = SPD;
   
   if (width > hudHeight/2.0) {
      width = hudHeight/2.0;
   }
   
   
   // this is messy.
   // We should use x and y offsets in Screen coordinates, etc
   // and pass them to the shader
   // Then we won't need any matrices, like in TTFRenderer.h --Mustafa
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(hudWidth - width, hudHeight - width, 0.0));
   modelMatrix.scale(width, width, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());

   hudShader->draw(string("speedometer"));
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(hudWidth - width/2.0, hudHeight - width/2.0, 0.0));
   modelMatrix.scale(width, width, 1.0);
   modelMatrix.rotate(30.0 * currentSpeed/10.0, vec3(0.0, 0.0, 1.0));
   modelMatrix.translate(vec3(-0.5, -0.5, 0.0));
   
   hudShader->usePlayerColor();
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->draw(string("needle"));
   
   hudShader->useTextureColor();

   hudShader->deactivate();
}

#define ENG_HGT 50
#define ENG_SCL 3.0
#define BCK_SCL 150.0
void GameHUD::drawEnergy(float maxEnergy, float energy, string name)
{
   
   if (name != "none") {
      string textName = name + "Energy";
      
      modelMatrix.loadIdentity();
      modelMatrix.translate(vec3(0.0, hudHeight - BCK_SCL, 0.0));
      modelMatrix.scale(BCK_SCL, BCK_SCL, 1.0);
      
      hudShader->setModelMatrix(modelMatrix.getMatrix());
      
      hudShader->draw(string(textName));
   }
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(0.0, hudHeight - ENG_HGT - 15.0, 0.0));
   modelMatrix.scale(maxEnergy * ENG_SCL, ENG_HGT, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->draw(string("energyBack"));
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(0.0, hudHeight - ENG_HGT - 15.0, 0.0));
   modelMatrix.scale(maxEnergy * ENG_SCL * (energy/maxEnergy), ENG_HGT, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->usePlayerColor();
   
   hudShader->draw(string("energy"));
   
   hudShader->useTextureColor();
   
   hudShader->deactivate();
}

#define PT_HGT 100.0
#define PT_WDT 175.0
void GameHUD::drawScore(int score)
{
   if (currentScore < score)
      currentScore++;
   
   float sclWdt = (score - currentScore) * 3.0;
   if (sclWdt > 25.0f) {
      sclWdt = 25.0f;
      currentScore = score - 25.0f / 3.0f;
   }
   
   float sclHgt = (sclWdt * (PT_HGT/PT_WDT)) * 3.0;
   
   modelMatrix.loadIdentity();
   modelMatrix.translate(vec3(hudWidth - PT_WDT - sclWdt + 2.0, 0.0, 0.0));
   modelMatrix.scale(PT_WDT + sclWdt, PT_HGT + sclHgt, 1.0);
   
   hudShader->setModelMatrix(modelMatrix.getMatrix());
   
   hudShader->usePlayerColor();
   
   hudShader->draw(string("score"));
   
   hudShader->useTextureColor();
   
   hudShader->deactivate();
}
