//
// main.cpp
// Final Project - Kart Part Park
// 
// Created by Bryan Bell, Eric Johnson, Mustafa, Chirs Pauley
// Date Created: 1/25/13
//
// For CSC476 - Real Time 3D Rendering

#include <GL/glfw.h>

#include <iostream>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctime>

// === helpful libraries ===
#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
using  glm::mat4;
using  glm::vec3;
using  glm::vec4;


#include "MStackHelp.h"

#include "PhongShader.h"
#include "HUDShader.h"
#include "FontShader.h"
#include "ModelManager.h"

#ifdef USE_DUMMY_SOUND
#include "DummySoundManager.h"
#else
#include "SDLSoundManager.h"
#endif

#include "GameDrawableObject.h"
#include "GameKartObject.h"
#include "GameCamera.h"
#include "GameRamp.h"
#include "GameBuilding.h"
#include "GameTerrain.h"

#include "GameUpgradesInclude.h"
#include "GameHUD.h"

#ifdef MAIN_USE_TTF
#include "TTFRenderer.h"
#else
#include "DummyTextRenderer.h"
#endif

#include "loadMap.h"
#include "calcFrustum.h"

#include "GameSettings.h"

#include "util.h"       // has randFloat()



using namespace std;

//-----------------------------------------------
// These are global state machines: 
// "physics_sim"-
//  Controls the movement and collisions of 
//   objects and as such each object will have a
//   pointer to an object of class type 
//   KPPPhysics::Actor which will control various
//   states of the object such as position
// "mesh_loader"-
//  This is a class I will make using my .obj 
//   loader code. It is passed, along with the
//   file name of the .obj file, to the 
//   KPPDrawnObject constructor.
// "shader"-
//  Shader object for the entire program. Pointer
//   is passed to "mesh_loader" and should not be
//   tampered with after that unless to change 
//   shader for all objects.
// "model_trans"-
//  This is the model-view translation matrix,
//   we might not need this given objects' 
//   independence from one another when drawn.
// NOTE: These objects should be the only of 
//  their type for the entire program. When 
//  initializing other objects please pass a 
//  pointer to these objects and do NOT create
//  duplicates.
//-----------------------------------------------



// === Globals =================================

// Singletons
// There should be only one instance of these
ModelManager *g_model_manager;
SoundManager *g_sound_manager;
TextRenderer *g_ttf_text_renderer;
GameSettings g_settings;

// just for the music
GameSound *g_music;
int nPlayers;

// test one object for now
PhongShader *meshShader;
HUDShader *hudShader;
vector<GameDrawableObject *> drawable_objects;
vector<GameKartObject *> kart_objects;

int g_num_players = 1;

// GLFW Window
int g_win_height, g_win_width;
int g_current_height, g_current_width;
int motionBlur = 0;
double g_time;
double g_last_time;
bool menu;
int selected = 0;
const char *mapSelected;

mat4 g_proj;
mat4 g_view;
RenderingHelper g_model_trans;

GameCamera *g_camera;

GameDrawableObject *skyBox;
int hasStarted = 0;




// === game info ===

int g_num_squashes = 0;
int g_timer = 120;

// *** lights ***

LightInfo g_lightInfo;



// This defines the array g_materials 
// leave this here - not a traditional .h file
#include "materials.h"


/* helper function to set up material for shading */
void setPhongMaterial(int i) {
    if ((i >= 0) && i < NUM_MATERIALS) {
        meshShader->setMaterial(&g_materials[i]);
    } else if (i == UNSET_MATERIAL) {
        meshShader->setMaterial(&g_materials[0]);
    } else if (i == MAGIC_MATERIAL) {
        meshShader->setMaterial(&g_materials[rand() % NUM_MATERIALS]);
    }
}

// *** end lights ***

// === end Globals ==============================



/* projection matrix */
void setProjectionMatrix(int kartIndex) {
   g_proj = glm::perspective( (float) kart_objects[kartIndex]->getPerspective(),
         (float)g_current_width/g_current_height, 0.1f, 250.f);
}
void setSkyboxProjectionMatrix() {
   g_proj = glm::perspective(45.0f, (float)g_current_width/g_current_height, 0.1f, 250.f);
}

/* camera controls */
void setView(int kartIndex) {
   vec3 kartDir = kart_objects[kartIndex]->getDirectionVector();
   vec3 kartPos = kart_objects[kartIndex]->getPosition();

   // move camera back and up
   kartDir = vec3(kartDir.x * 6.0, kartDir.y - 2.0, kartDir.z * 6.0);

   g_camera->setLookAtTarget(kart_objects[kartIndex]->getPosition());
   g_camera->setPosition(kartPos - kartDir);

   g_view = g_camera->getViewMat();
   
}

void getInputState()
{
   for (int i = 0; i < (int)kart_objects.size(); i++) {
      float joy[4]; //should vary from -1.0 to 1.0
      unsigned char button[32]; //either GLFW_PRESSED or GLFW_RELEASED
      
      if (kart_objects[i]->isUsingController()) {
         glfwGetJoystickPos(i, joy, 4);
         glfwGetJoystickButtons(i, button, 32);
      } else {
         if (glfwGetKey(kart_objects[i]->getInput(0)) == GLFW_PRESS)
            joy[3] = 1.0;
         else if (glfwGetKey(kart_objects[i]->getInput(1)) == GLFW_PRESS)
            joy[3] = -1.0;
         else
            joy[3] = 0.0;
         
         if (glfwGetKey(kart_objects[i]->getInput(2)) == GLFW_PRESS)
            joy[0] = 1.0;
         else if (glfwGetKey(kart_objects[i]->getInput(3)) == GLFW_PRESS)
            joy[0] = -1.0;
         else
            joy[0] = 0.0;
         
         button[0] = glfwGetKey(kart_objects[i]->getInputMap().action);
         button[1] = glfwGetKey(kart_objects[i]->getInputMap().cycleActive);
         button[2] = glfwGetKey(kart_objects[i]->getInputMap().cycleFront);
         button[3] = glfwGetKey(kart_objects[i]->getInputMap().cycleSide);
         button[4] = glfwGetKey(kart_objects[i]->getInputMap().cycleBack);
      }
      
      //printf("Joy: %0.3f\n", joy[3]);
      kart_objects[i]->setJoystickState(joy); //These functions are commented out in GameKartObject *****
      kart_objects[i]->setButtonState(button);//Update internal input arrays in kartObject, then allow it to update based on given input *****
   }
}


void update(double dt)
{
   getInputState();

	//ExtractFrustum();
   
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      drawable_objects[i]->update(dt);
   }
   

   // test for collisions
   /*
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      for (int j = i+1; j < (int)drawable_objects.size(); j++) {
         if (g_model_manager->boxOnBox(drawable_objects[i]->getBoundingInfo(),
                drawable_objects[j]->getBoundingInfo())) {
            drawable_objects[i]->onCollide(drawable_objects[j]);
            drawable_objects[j]->onCollide(drawable_objects[i]);
         }
      }
   }
   */

   // only test kart objects with drawable objects
   for (int k = 0; k < (int)kart_objects.size(); k++) {
      for (int j = 0; j < (int)drawable_objects.size(); j++) {
         // don't test collision with self
         if (kart_objects[k] == drawable_objects[j]) {
            continue;
         }

         if (g_model_manager->boxOnBox(kart_objects[k]->getBoundingInfo(),
                drawable_objects[j]->getBoundingInfo())) {
            kart_objects[k]->onCollide(drawable_objects[j]);
            // both karts will detect the collision so only call on self if another kart
            if (!dynamic_cast<GameKartObject *>(drawable_objects[j])) {
               drawable_objects[j]->onCollide(kart_objects[k]);
            }
         }
      }
   }
   
   for (std::vector<GameDrawableObject *>::iterator it = drawable_objects.begin();
         it != drawable_objects.end();) {
      if ((*it)->isScheduledForRemoval()) {
         printf("erasing: %s\n", (*it)->getName());
         // it now points to the next obj
         it = drawable_objects.erase(it);
      } else {
         it++;
      }
   }
}

void drawSkyBox()
{
   glDisable(GL_DEPTH_TEST);
   glDepthMask(false);

   skyBox->setPosition(glm::vec3(0,0,0));
   skyBox->setScale(glm::vec3(0.5, 0.5, 0.5));
   skyBox->draw(meshShader, g_model_trans);

   glDepthMask(true);
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
}



void draw(float dt, int kartIndex)
{
   glClearColor (0.8f, 0.8f, 1.0f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   

   // set once for this shader

   //setSkyboxProjectionMatrix();
      setProjectionMatrix(kartIndex);
   meshShader->use();
   meshShader->setProjMatrix(g_proj); 
   meshShader->setViewMatrix(g_view);

  
   drawSkyBox();


   setProjectionMatrix(kartIndex);
   setView(kartIndex);

   meshShader->use();
   meshShader->setProjMatrix(g_proj);
   meshShader->setViewMatrix(g_view);

   // get camera position
   vec3 kartPos = kart_objects[kartIndex]->getPosition();
   vec3 kartDir = normalize(kart_objects[kartIndex]->getDirectionVector());
   kartDir = vec3(kartDir.x * 3.0, kartDir.y * 3.0 - 2.0, kartDir.z * 3.0);
   meshShader->setCamPos(kartPos - kartDir);

   ExtractFrustum(); 

   // draw objects
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      if(SphereInFrustum(drawable_objects[i]->getPosition(), 
                         drawable_objects[i]->getBoundingInfo().radius * 1.5) > 0) {
         setPhongMaterial(drawable_objects[i]->getMaterialIndex());
         drawable_objects[i]->draw(meshShader, g_model_trans);
      } else { //printf("Not being drawn\n");
         /*
         // test frustum on one object.
         if (dynamic_cast<GamePartWings *>(drawable_objects[i]))
            printf("obj not drawn: %s\n", drawable_objects[i]->getName());
         // printf("not being drawn %f %f %f\n",drawable_objects[i]->getPosition().x,drawable_objects[i]->getPosition().y,drawable_objects[i]->getPosition().z);
         */
      }
   }

   // draw text
   //char text[100];
   /*sprintf(text, "speed: %.1f", kart_objects[kartIndex]->getSpeed());
   g_ttf_text_renderer->drawText(text, -0.95, 0.8, 2.0/g_current_width, 2.0/g_current_height);*/

   // draw squashes
   /*sprintf(text, "points: %d", kart_objects[kartIndex]->getPoints());
   g_ttf_text_renderer->drawText(text, 0.2, 0.8, 2.0/g_current_width, 2.0/g_current_height);*/
   
   // draw fps
   /*sprintf(text, "fps: %.0f", 1/dt);
   g_ttf_text_renderer->drawText(text, 0.4, 0.6, 2.0/g_current_width, 2.0/g_current_height);*/
   
   /* draw height
   sprintf(text, "height: %.1f", kart_objects[kartIndex]->getPosition().y-kart_objects[0]->getRideHeight());
   g_ttf_text_renderer->drawText(text, -0.95, 0.6, 2.0/g_current_width, 2.0/g_current_height);*/

   /* draw energy
   sprintf(text, "energy: %.1f", kart_objects[kartIndex]->getEnergy());
   g_ttf_text_renderer->drawText(text, -0.95, -0.8, 2.0/g_current_width, 2.0/g_current_height);*/
   
   
}

void gameMenu()
{
   glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glDisable(GL_DEPTH_TEST);
   glAlphaFunc(GL_GREATER,0.1f);
   glEnable(GL_ALPHA_TEST);


   char text[100];

    

   sprintf(text, "Kart Part Park");
   g_ttf_text_renderer->drawText(text, -0.75, 0.75, 2.0/g_current_width, 2.0/g_current_height); 

   if(selected == 0){
   sprintf(text, "Begin 1P game");
   g_ttf_text_renderer->drawText(text, -0.75, 0.50, 5.0/g_current_width, 5.0/g_current_height);
   }
   else{
   sprintf(text, "Begin 1P game");
   g_ttf_text_renderer->drawText(text, -0.75, 0.50, 2.0/g_current_width, 2.0/g_current_height);   
   }

   if(selected == 2){
   sprintf(text, "Begin 2P game");
   g_ttf_text_renderer->drawText(text, -0.75, 0.25, 5.0/g_current_width, 5.0/g_current_height);
   }
   else{
   sprintf(text, "Begin 2P game");
   g_ttf_text_renderer->drawText(text, -0.75, 0.25, 2.0/g_current_width, 2.0/g_current_height);   
   }

   if(selected == 4){
   sprintf(text, "Music Volume: " );
   g_ttf_text_renderer->drawText(text, -0.75, 0.0, 5.0/g_current_width, 5.0/g_current_height);
   }else{
   sprintf(text, "Music Volume: " );
   g_ttf_text_renderer->drawText(text, -0.75, 0.0, 2.0/g_current_width, 2.0/g_current_height);   
   }

   if(selected == 6){
   sprintf(text, "Sound Volume: ");
   g_ttf_text_renderer->drawText(text, -0.75, -0.25, 5.0/g_current_width, 5.0/g_current_height);
   }
   else{
   sprintf(text, "Sound Volume: ");
   g_ttf_text_renderer->drawText(text, -0.75, -0.25, 2.0/g_current_width, 2.0/g_current_height);  
   }

   if(selected == 8){
   sprintf(text, "Quit Game");
   g_ttf_text_renderer->drawText(text, -0.75, -0.50, 5.0/g_current_width, 5.0/g_current_height);
   }
   else{
   sprintf(text, "Quit Game");
   g_ttf_text_renderer->drawText(text, -0.75, -0.50, 2.0/g_current_width, 2.0/g_current_height);   
   }
   
    
 
   
   
  glfwSwapBuffers();  
   //glDisable(GL_BLEND);
   glDisable(GL_ALPHA_TEST);
   glEnable(GL_DEPTH_TEST);

}

void drawHUD (int kartIndex) {
   glDisable(GL_DEPTH_TEST);
   glAlphaFunc(GL_GREATER,0.1f);
   glEnable(GL_ALPHA_TEST);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glEnable(GL_BLEND);
   
   //setOrthographicMatrix();
   //setHUDView();
   
   //meshShader->setProjMatrix(g_proj);
   //meshShader->setViewMatrix(g_view);
   //glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
   //glClear( GL_COLOR_BUFFER_BIT );meshShader->setMaterial(&meshStorage.material[i]);
   
   kart_objects[kartIndex]->drawHUD();
   
   //glDisable(GL_BLEND);
   glDisable(GL_ALPHA_TEST);
   glEnable(GL_DEPTH_TEST);
   
}

void drawMultipleViews(double dt) {
   int kartIndex = 0;
   for (int i = 0; i * g_current_height < g_win_height; i++) {
      for (int j = 0; j * g_current_width < g_win_width; j++) {
         glViewport( j * g_current_width, i * g_current_height, g_current_width, g_current_height );
         glScissor( j * g_current_width, i * g_current_height, g_current_width, g_current_height );
         
         if (kartIndex < (int)kart_objects.size()) {
               
               g_camera->setPosition(glm::vec3(0,0,0));
               vec3 v = kart_objects[kartIndex]->getDirectionVector();
   g_camera->setLookAtTarget(glm::vec3(v.x,v.y,v.z)); 

   g_view = g_camera->getViewMat();
    
            draw(dt, kartIndex);

            if(motionBlur == 1) {
               glAccum(GL_MULT, .9);
               glAccum(GL_ACCUM, 1-.9);
               glAccum(GL_RETURN, 1.0);
               glFlush();
            }
            drawHUD(kartIndex);
            char text[100];
            sprintf(text, "%d", kart_objects[kartIndex]->getPoints());
            g_ttf_text_renderer->drawText(text, 0.65, 0.8, 2.0/g_current_width, 2.0/g_current_height);
            kartIndex++;
         } else {
            glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            //g_proj = mat4(1.0);
         }
      }
   }
}



void gameLoop()
{
   double dt;
   int time_then = 0;;
   int time_now = 0;
   g_time = glfwGetTime();

   if (g_timer > 0){ 
    time_then = time(0);
   }
   dt = g_time - g_last_time;

   update(dt);
   
   drawMultipleViews(dt);
   
   char text[100];
   
   if (g_num_players == 1) {
      sprintf(text, "%d", g_timer);
      g_ttf_text_renderer->drawText(text, -0.1, 0.85, 2.0/g_current_width, 2.0/g_current_height);
   } else {
      sprintf(text, "%d", g_timer);
      g_ttf_text_renderer->drawText(text, -0.1, -1.0, 2.0/g_current_width, 2.0/g_current_height);
   }
   
   glfwSwapBuffers();

   g_last_time = g_time;  

   if (g_timer > 0){
    time_now = time(0);
   }
  
   if(g_timer > 0){
   g_timer -=time_now - time_then;
   }

   if(g_timer == 0 && (int)kart_objects.size() == 2)
   {
      if (kart_objects[0]->getPoints() > kart_objects[1]->getPoints())
      {   kart_objects[0]->win();
         kart_objects[1]->lose();}
      else if (kart_objects[0]->getPoints() < kart_objects[1]->getPoints())
        { kart_objects[0]->lose();
         kart_objects[1]->win();}
            
   }
   
   if(g_timer < -5.0 &&  g_num_players == 2) {
      g_timer = 120;
      kart_objects[0]->reset();
      kart_objects[1]->reset();
   }
}



void initMap(const char *filename) {
   
   loadMap(filename, drawable_objects);
   
   
}

void initObjects(const char *map) {
   cout << "Initializing game objects\n";
   
   // TODO - test for return values (but we usually know if they work or not)
   g_camera = new GameCamera();
   
   hudShader = new HUDShader();
   meshShader = new PhongShader();
   // Light 
   g_lightInfo.pos = vec3(1, 50, 1);
   g_lightInfo.color = vec3(1.0f, 1.0f, 1.0f); 

   meshShader->setLight(g_lightInfo);
   meshShader->setShowNormals(0);

   
   
   //floors
   GameTerrain *floor = new GameTerrain();
   floor->setScale(vec3(100.0, 1.0, 100.0));
   floor->setPosition(vec3(0, 0.0, 0));
   drawable_objects.push_back(floor);

   
   // karts
   int num_players_from_settings = nPlayers;
   
   if (num_players_from_settings > 0) {
      g_num_players = num_players_from_settings;
   }
   
   if (g_num_players == 1) {
      g_current_height = g_win_height;
      g_current_width = g_win_width;
   } else if (g_num_players == 2) {
      g_current_height = g_win_height/2;
      g_current_width = g_win_width;
   } else if (g_num_players == 3 || g_num_players == 4) {
      g_current_height = g_win_height/2;
      g_current_width = g_win_width/2;
   }
   
   // hax
   // 1st kart
   if (g_num_players >= 1) {
      GameKartObject *kart = new GameKartObject("models/ball.obj");
      kart->setPosition(vec3(30, 10.0, 5));
      kart->setScale(vec3(1.0, 0.75, 1.0));
      kart->setDirection(180);
      kart->setInputMap('W', 'S', 'A', 'D', ' ', '1', '2', '3', '4');
      kart->resize(g_current_width, g_current_height);
      kart->setHUDColor(vec3(1.0, 0.0, 0.0));
      drawable_objects.push_back(kart);
      kart_objects.push_back(kart);
   }
   if (g_num_players >= 2) {
      GameKartObject *otherKart = new GameKartObject("models/ball.obj");
      otherKart->setPosition(vec3(45, 10.0, 0));
      otherKart->setScale(vec3(1.0, 0.75, 1.0));
      otherKart->setDirection(0);
      otherKart->setInputMap(GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_ENTER, '7', '8', '9', '0');
      otherKart->resize(g_current_width, g_current_height);
      otherKart->setHUDColor(vec3(1.0, 1.0, 0.0));
      drawable_objects.push_back(otherKart);
      kart_objects.push_back(otherKart);
   }
   if (g_num_players >= 3) {
      GameKartObject *thirdKart = new GameKartObject("models/ball.obj");
      thirdKart->setPosition(vec3(30, 1, 45));
      thirdKart->setScale(vec3(1.0, 0.75, 1.0));
      thirdKart->setDirection(0);
      thirdKart->resize(g_current_width, g_current_height);
      drawable_objects.push_back(thirdKart);
      kart_objects.push_back(thirdKart);
   }
   
   
   if (map != NULL) {
      initMap(map);
   }
   else {
      // Thingies
      
      //upgrades
      GamePartUpgrade *part = new GamePartWings();
      part->setPosition(vec3(5, 1, 2));
      part->setScale(vec3(2.0, 1.0, 1.0));
      drawable_objects.push_back(part);
      
      part = new GamePartEngine();
      part->setPosition(vec3(25, 1, 10));
      part->setScale(vec3(2.0, 1.0, 1.0));
      drawable_objects.push_back(part);
      
      part = new GamePartBattery();
      part->setPosition(vec3(35, 1, 35));
      part->setScale(vec3(2.0, 1.0, 1.0));
      drawable_objects.push_back(part);
      
      GameStatUpgrade *stat = new GameStatSpeed();
      stat->setPosition(vec3(10, 1, 10));
      stat->setScale(vec3(2.0, 1.0, 1.0));
      drawable_objects.push_back(stat);
      
      GameActiveUpgrade *active = new GameActiveBoost();
      active->setPosition(vec3(10, 1, 5));
      active->setScale(vec3(2.0, 1.0, 1.0));
      drawable_objects.push_back(active);
      
      active = new GameActiveJetpack();
      active->setMaterialIndex(MAGIC_MATERIAL);
      active->setPosition(vec3(10, 1, 25));
      active->setScale(vec3(1.0, 1.0, 1.0));
      drawable_objects.push_back(active);
      
      
      active = new GameActiveJetpack();
      active->setMaterialIndex(MAGIC_MATERIAL);
      active->setPosition(vec3(-10, 1, 25));
      active->setScale(vec3(1.0, 1.0, 1.0));
      drawable_objects.push_back(active);
      
      active = new GameActiveTurning();
      active->setPosition(vec3(25, 1, 25));
      active->setScale(vec3(1.0, 1.0, 1.0));
      drawable_objects.push_back(active);
      
      
      
      GameRamp *ramp = new GameRamp();
      ramp->setPosition(vec3(-25, 2, -25));
      ramp->setScale(vec3(3.0, 2.0, 3.0));
      drawable_objects.push_back(ramp);
      
      GameBuilding *buildin = new GameBuilding();
      buildin->setPosition(vec3(-25, 2, 0));
      buildin->setScale(vec3(10.0, 2.0, 10.0));
      drawable_objects.push_back(buildin);
      
      
      /* GamePhysicalObject *building = new GamePhysicalObject("cube");
       building->setName("building");
       building->setPosition(vec3(0, 2, 0));
       building->setScale(vec3(1.0, 1.0, 1.0));
       drawable_objects.push_back(building);*/
      
      /*GameKartObject *kart = new GameKartObject("Kart");
       if (glfwGetJoystickParam(kart_objects.size(), GLFW_PRESENT) == GL_TRUE) { // What code should look like for Kart Objects *****
       printf("Controller Connected for Player %d\n", kart_objects.size());   // Please uncomment and test                   *****
       kart->setUsingController(true);
       }
       kart_objects.push_back(kart);
       drawable_objects.push_back(kart);*/
      
   }
   
   for (int i = 0; i < 100; i++) {
      GameDrawableObject *object = new GameDrawableObject("models/squash.obj");
      //object->setName("thingy");
      object->setPosition(vec3(200*randFloat() - 100.0, 1.0, 200*randFloat() - 100.0));
      object->setScale(vec3(0.5, 0.5, 0.5));
      drawable_objects.push_back(object);
   }

   // set initial materials for objects with unset materials
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      // if not set
      if (drawable_objects[i]->getMaterialIndex() == UNSET_MATERIAL) {
         drawable_objects[i]->setMaterialIndex(i % NUM_MATERIALS);
      }
   }

}


void initialize(const char *map)
{
   /* === Start OpenGL Initialization === */
   glClearColor (0.8f, 0.8f, 1.0f, 1.0f);
   // Black Background
   glClearDepth (1.0f);    // Depth Buffer Setup
   glDepthFunc (GL_LEQUAL);    // The Type Of Depth Testing
   glEnable (GL_DEPTH_TEST);// Enable Depth Testing
   glEnable( GL_SCISSOR_TEST );// Enable Scissor Test
   /* texture specific settings */
   glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   /* === End OpenGL Initialization === */

   g_model_trans.useModelViewMatrix();
   g_model_trans.loadIdentity();

   g_model_manager = new ModelManager();



#ifdef USE_DUMMY_SOUND
   g_sound_manager = new DummySoundManager();
   g_music = g_sound_manager->getMusic("no music");   
   g_music->play();
#else

   g_sound_manager = new SDLSoundManager();

   if (g_settings["play_music"] == 1) {
      printf("Music enabled\n");
      g_music = g_sound_manager->getMusic("music/raptor.ogg");
      g_music->play();
   }
#endif

   // initialize with default font
#ifdef MAIN_USE_TTF
   g_ttf_text_renderer = new TTFRenderer("fonts/DejaVuSans.ttf");
#else
   g_ttf_text_renderer = new DummyTextRenderer("lolololol");
#endif

   
   printf("one: %d\n", g_settings["one"]);
   printf("void: %d\n", g_settings["lol"]);
   printf("three: %d\n", g_settings["three"]);

   srand(0xdeadf00d);

   mapSelected = map;
   //initObjects(map);
}



void shutdown() {

   //delete g_sound_manager;
   exit(0);
}


// For GLFW
void reshape(int width, int height)
{
   g_win_width = width;
   g_win_height = height;
   
   if (g_num_players == 1) {
      g_current_height = g_win_height;
      g_current_width = g_win_width;
   } else if (g_num_players == 2) {
      g_current_height = g_win_height/2;
      g_current_width = g_win_width;
   } else if (g_num_players == 3 || g_num_players == 4) {
      g_current_height = g_win_height/2;
      g_current_width = g_win_width/2;
   }
   
   g_time = glfwGetTime();
   g_last_time = glfwGetTime();
}


// Keyboard callback

void GLFWCALL keyboard_callback_key(int key, int action) {
   // only gives upercase
   switch (key) {
   case 'R':
      g_camera->setYaw(45);
      printf("yaw %f\n", g_time - g_last_time);
      break;
   case 'Q':
   case GLFW_KEY_ESC:
      printf("Bye :)\n");
      shutdown();
      break;
   case 'W':
      if(menu == true){
         if(selected > 0)
         {selected -= 1;}
      }
      break;
   case 'S':
      if(menu == true){
         if(selected < 8)
         {selected += 1;}
      }     
      break;
   case GLFW_KEY_ENTER:
      if(menu == true && action == GLFW_RELEASE){
         if(selected == 0)
         {menu = false;
          nPlayers = 1;
            glfwSetTime(0.0);
            g_last_time = glfwGetTime();
          initObjects(mapSelected);
          }
         if(selected == 2){
          menu = false;
          nPlayers = 2;
            glfwSetTime(0.0);
            g_last_time = glfwGetTime();
          initObjects(mapSelected);
         }
         if(selected == 8)
         {shutdown();}
      
      }
      break; 
   }

   /*
   switch (action) {
   case GLFW_PRESS:
      break;

   case GLFW_RELEASE:
      break;
   }
   */
}


int main(int argc, char** argv) 
{
   // default values. Will be overwritten by game settings
   g_win_width = 640;
   g_win_height = 480;

	srandom(time(NULL));
   
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   // can't do anything if this fails
   if (!glfwInit()) {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      exit( EXIT_FAILURE );
   }

   
   g_settings.load("game_settings.ini");
   int new_width = g_settings["win_width"];
   int new_height = g_settings["win_height"];

   g_win_width = new_width == -1 ? g_win_width : new_width;
   g_win_height = new_height == -1 ? g_win_height : new_height;

   if (!glfwOpenWindow(g_win_width, g_win_height, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
      fprintf( stderr, "Failed to open GLFW window\n" );
      glfwTerminate();
      exit( EXIT_FAILURE );
   } else {
      // in GLSL_helper
      getGLversion();
	}

   glfwSetWindowSizeCallback( reshape );
   glfwSetKeyCallback( keyboard_callback_key );

   initialize(argc == 2 ? argv[1] : NULL);

   /*if (argc == 2) {
      initMap(argv[1]);
   }*/

   glfwSetTime(0.0);
   g_last_time = glfwGetTime();

   skyBox = new GameDrawableObject("models/ball.obj");
   menu = true;

   while (glfwGetWindowParam(GLFW_OPENED)) {
      if(menu == false)
      gameLoop();
      else
      gameMenu();
   }

   glfwTerminate();
   return 0;
}


// vim modeline
// vim: set sw=3 ts=3 :

