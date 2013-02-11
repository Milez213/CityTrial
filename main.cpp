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
#include <time.h>

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

#include "GamePartWings.h"
#include "GameStatSpeed.h"
#include "GameActiveBoost.h"
#include "GameHUD.h"

#ifdef MAIN_USE_TTF
#include "TTFRenderer.h"
#else
#include "DummyTextRenderer.h"
#endif

#include "loadMap.h"
//#include "frustum/FrustumG.h"

#include "GameSettings.h"

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


// test one object for now
PhongShader *meshShader;
vector<GameDrawableObject *> drawable_objects;
vector<GameKartObject *> kart_objects;

int g_num_players = 1;


RenderingHelper g_model_trans;

// GLFW Window
int g_win_height, g_win_width;
int g_current_height, g_current_width;
int motionBlur = 0;
double g_time;
double g_last_time;

mat4 g_proj;
mat4 g_view;
mat4 g_model;

GameCamera *g_camera;


GLuint fbo;



// === game info ===

int g_num_squashes = 0;


// *** lights ***

LightInfo g_lightInfo;

#define NUM_MATERIALS 5

PhongMaterial g_materials[NUM_MATERIALS] = {
                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(H2_3f(0x9d5900)), // diff
                   vec3(1, 0, 0),       // spec
                   20.0},               // shine

                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(H2_3f(0xe4000c)), // diff
                   vec3(1, 1, 1),       // spec
                   20.0},               // shine

                  {vec3(0.1, 0.1, 0.1),
                   vec3(H2_3f(0xfff852)), //Hex color to rgb
                   vec3(1, 1, 1),
                   5.0},

                  {vec3(0.3, 0.3, 0.3),
                   vec3(0, 0, 1),
                   vec3(0.1, 0.1, 0.7),
                   30.0},
                  {vec3(.1, .8, .1),  // for drawing light
                   vec3(0.1, 0.9, .1),
                   vec3(3),
                   20.0},
};

/* helper function to set up material for shading */

float fr[6][4];

void ExtractFrustum()
{
   float   clip[16];
   float   t;


  mat4 tpose = g_proj * g_view;
  tpose = glm::transpose(tpose);

  mat4 g_mView =  tpose * g_model;


   clip[ 0] = g_mView[ 0][0]; 
   clip[ 1] = g_mView[ 1][0] ;
   clip[ 2] = g_mView[ 2][0] ;
   clip[ 3] = g_mView[ 3][0] ;

   clip[ 4] = g_mView[ 0][1] ;
   clip[ 5] = g_mView[ 1][1] ;
   clip[ 6] = g_mView[ 2][1] ;
   clip[ 7] = g_mView[ 3][1] ;

   clip[ 8] = g_mView[ 0][2];
   clip[ 9] = g_mView[ 1][2] ;
   clip[10] = g_mView[ 2][2];
   clip[11] = g_mView[ 3][2] ;

   clip[12] = g_mView[0][3] ;
   clip[13] = g_mView[1][3] ;
   clip[14] = g_mView[2][3] ;
   clip[15] = g_mView[3][3] ;
   

   /* Combine the two matrices (multiply projection by modelview) */
   /*clip[ 0] = g_mView[ 0][0] * g_proj[ 0][0] + g_mView[ 1][0] * g_proj[ 1][0] + g_mView[ 2][0] * g_proj[ 2][0] + g_mView[ 3][0] * g_proj[3][0];
   clip[ 1] = g_mView[ 0][0] * g_proj[ 0][1] + g_mView[ 1][0] * g_proj[ 1][1] + g_mView[ 2][0] * g_proj[ 2][1] + g_mView[ 3][0] * g_proj[3][1];
   clip[ 2] = g_mView[ 0][0] * g_proj[ 0][2] + g_mView[ 1][0] * g_proj[ 1][2] + g_mView[ 2][0] * g_proj[2][2] + g_mView[ 3][0] * g_proj[3][2];
   clip[ 3] = g_mView[ 0][0] * g_proj[ 0][3] + g_mView[ 1][0] * g_proj[ 1][3] + g_mView[ 2][0] * g_proj[2][3] + g_mView[ 3][0] * g_proj[3][3];

   clip[ 4] = g_mView[ 0][1] * g_proj[ 0][0] + g_mView[ 1][1] * g_proj[ 1][0] + g_mView[ 2][1] * g_proj[ 2][0] + g_mView[ 3][1] * g_proj[3][0];
   clip[ 5] = g_mView[ 0][1] * g_proj[ 0][1] + g_mView[ 1][1] * g_proj[ 1][1] + g_mView[ 2][1] * g_proj[ 2][1] + g_mView[ 3][1] * g_proj[3][1];
   clip[ 6] = g_mView[ 0][1] * g_proj[ 0][2] + g_mView[ 1][1] * g_proj[ 1][2] + g_mView[ 2][1] * g_proj[2][2] + g_mView[ 3][1] * g_proj[3][2];
   clip[ 7] = g_mView[ 0][1] * g_proj[ 0][3] + g_mView[ 1][1] * g_proj[ 1][3] + g_mView[ 2][1] * g_proj[2][3] + g_mView[ 3][1] * g_proj[3][3];

   clip[ 8] = g_mView[ 0][2] * g_proj[0][0] + g_mView[ 1][2] * g_proj[ 1][0] + g_mView[2][2] * g_proj[ 2][0] + g_mView[3][2] * g_proj[3][0];
   clip[ 9] = g_mView[ 0][2] * g_proj[0][1] + g_mView[ 1][2] * g_proj[ 1][1] + g_mView[2][2] * g_proj[ 2][1] + g_mView[3][2] * g_proj[3][1];
   clip[10] = g_mView[ 0][2] * g_proj[0][2] + g_mView[ 1][2] * g_proj[ 1][2] + g_mView[2][2] * g_proj[2][2] + g_mView[3][2] * g_proj[3][2];
   clip[11] = g_mView[ 0][2] * g_proj[0][3] + g_mView[ 1][2] * g_proj[ 1][3] + g_mView[2][2] * g_proj[2][3] + g_mView[3][2] * g_proj[3][3];

   clip[12] = g_mView[0][3] * g_proj[0][0] + g_mView[1][3] * g_proj[ 1][0] + g_mView[2][3] * g_proj[ 2][0] + g_mView[3][3] * g_proj[3][0];
   clip[13] = g_mView[0][3] * g_proj[0][1] + g_mView[1][3] * g_proj[ 1][1] + g_mView[2][3] * g_proj[ 2][1] + g_mView[3][3] * g_proj[3][1];
   clip[14] = g_mView[0][3] * g_proj[0][2] + g_mView[1][3] * g_proj[ 1][2] + g_mView[2][3] * g_proj[2][2] + g_mView[3][3] * g_proj[3][2];
   clip[15] = g_mView[0][3] * g_proj[0][ 3] + g_mView[1][3] * g_proj[ 1][3] + g_mView[2][3] * g_proj[2][3] + g_mView[3][3] * g_proj[3][3];
   
   /* Extract the numbers for the RIGHT plane */
   fr[0][0] = clip[ 3] - clip[ 0];
   fr[0][1] = clip[ 7] - clip[ 4];
   fr[0][2] = clip[11] - clip[ 8];
   fr[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( fr[0][0] * fr[0][0] + fr[0][1] * fr[0][1] + fr[0][2] * fr[0][2] );
   fr[0][0] /= t;
   fr[0][1] /= t;
   fr[0][2] /= t;
   fr[0][3] /= t;
   
   /* Extract the numbers for the LEFT plane */
   fr[1][0] = clip[ 3] + clip[ 0];
   fr[1][1] = clip[ 7] + clip[ 4];
   fr[1][2] = clip[11] + clip[ 8];
   fr[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( fr[1][0] * fr[1][0] + fr[1][1] * fr[1][1] + fr[1][2] * fr[1][2] );
   fr[1][0] /= t;
   fr[1][1] /= t;
   fr[1][2] /= t;
   fr[1][3] /= t;

   /* Extract the BOTTOM plane */
   fr[2][0] = clip[ 3] + clip[ 1];
   fr[2][1] = clip[ 7] + clip[ 5];
   fr[2][2] = clip[11] + clip[ 9];
   fr[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( fr[2][0] * fr[2][0] + fr[2][1] * fr[2][1] + fr[2][2] * fr[2][2] );
   fr[2][0] /= t;
   fr[2][1] /= t;
   fr[2][2] /= t;
   fr[2][3] /= t;

   /* Extract the TOP plane */
   fr[3][0] = clip[ 3] - clip[ 1];
   fr[3][1] = clip[ 7] - clip[ 5];
   fr[3][2] = clip[11] - clip[ 9];
   fr[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( fr[3][0] * fr[3][0] + fr[3][1] * fr[3][1] + fr[3][2] * fr[3][2] );
   fr[3][0] /= t;
   fr[3][1] /= t;
   fr[3][2] /= t;
   fr[3][3] /= t;

   /* Extract the FAR plane */
   fr[4][0] = clip[ 3] - clip[ 2];
   fr[4][1] = clip[ 7] - clip[ 6];
   fr[4][2] = clip[11] - clip[10];
   fr[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( fr[4][0] * fr[4][0] + fr[4][1] * fr[4][1] + fr[4][2] * fr[4][2] );
   fr[4][0] /= t;
   fr[4][1] /= t;
   fr[4][2] /= t;
   fr[4][3] /= t;

   /* Extract the NEAR plane */
   fr[5][0] = clip[ 3] + clip[ 2];
   fr[5][1] = clip[ 7] + clip[ 6];
   fr[5][2] = clip[11] + clip[10];
   fr[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( fr[5][0] * fr[5][0] + fr[5][1] * fr[5][1] + fr[5][2] * fr[5][2] );
   fr[5][0] /= t;
   fr[5][1] /= t;
   fr[5][2] /= t;
   fr[5][3] /= t;


/*for (int i = 0; i<6;i++)
{
   for (int d = 0; d<4; d++)
   {
      printf("%f ",fr[i][d]);
   }
   printf("\n");
}
printf("\n");*/
}




void setPhongMaterial(int i) {
    if ((i >= 0) && i < NUM_MATERIALS) {
        meshShader->setMaterial(g_materials[i]);
    }
}


// *** end lights ***

// === end Globals ==============================


// returns in range -1, 1 (not sure if incusive)
float randFloat() {
       return ((float) rand() / RAND_MAX);
}


/* projection matrix */

void setProjectionMatrix(int kartIndex) {
   g_proj = glm::perspective( (float) kart_objects[kartIndex]->getSpeed() * 0.5f + 90.0f,
         (float)g_current_width/g_current_height, 0.1f, 100.f);
}

void setOrthographicMatrix() {
   g_proj = glm::ortho(0.0f, (float)g_current_width, (float)g_current_height, 0.0f, -1.0f, 1.0f);

}


/* camera controls */
void setView(int kartIndex) {
   vec3 kartDir = kart_objects[kartIndex]->getDirectionVector();
   vec3 kartPos = kart_objects[kartIndex]->getPosition();

   // move camera back and up
   kartDir = vec3(kartDir.x * 3.0, kartDir.y * 3.0 - 2.0, kartDir.z * 3.0);

   g_camera->setLookAtTarget(kart_objects[kartIndex]->getPosition());
   g_camera->setPosition(kartPos - kartDir);

   g_view = g_camera->getViewMat();
   
}

void setHUDView() {
   g_view = glm::lookAt( glm::vec3( 0.0f, 0.0f, 2.0f ),glm::vec3( 0.0f, 0.0f, 0.0f ),glm::vec3( 0.0f, 1.0f, 0.0f ) );
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
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      for (int j = i+1; j < (int)drawable_objects.size(); j++) {
         if (g_model_manager->boxOnBox(drawable_objects[i]->getBoundingInfo(),
                drawable_objects[j]->getBoundingInfo())) {
            drawable_objects[i]->onCollide(drawable_objects[j]);
            drawable_objects[j]->onCollide(drawable_objects[i]);
         }
      }
   }
   
   for (std::vector<GameDrawableObject *>::iterator it = drawable_objects.begin(); it != drawable_objects.end(); ++it) {
      if ((*it)->isScheduledForRemoval()) {
         drawable_objects.erase(it);
      }
   }
   



   


   
}

   bool checkFrust(vec3 pos)
   {
   int p;

   for( p = 0; p < 6; p++ )
      if( fr[p][0] * pos.x + fr[p][1] * pos.y + fr[p][2] * pos.z + fr[p][3] < 0.0 ){
         //printf("%d\n",p);
         return false;
      }
   return true;
   }



float SphereInFrustum( vec3 pos, float radius    )
{
  int p;
  float d;
int c = 0;

  for( p = 0; p < 6; p++ )
  {
    d = fr[p][0] * pos.x + fr[p][1] * pos.y + fr[p][2] * pos.z + fr[p][3];
    if( d <= -radius )
      return 0;
    if( d > radius )
      c++;
  }
  return (c == 6) ? 2 : 1;
}


void draw(float dt, int kartIndex)
{
   glClearColor (0.8f, 0.8f, 1.0f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
   setProjectionMatrix(kartIndex);
   setView(kartIndex);

   // set once for this shader
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
      setPhongMaterial(i%NUM_MATERIALS);
      if(SphereInFrustum(drawable_objects[i]->getPosition(),drawable_objects[i]->getBoundingInfo().radius * 1.5) >0){
      
      drawable_objects[i]->draw(meshShader, g_model_trans);
      }
      else
      {
      //printf("not being drawn %f %f %f\n",drawable_objects[i]->getPosition().x,drawable_objects[i]->getPosition().y,drawable_objects[i]->getPosition().z);
      }
   }


   /* psuedocode
   for each (KKPDrawnObject object in drawn_objects) {
      object->draw(model_trans);
   }
   */

   // draw text
   char text[100];
   sprintf(text, "speed: %.1f", kart_objects[kartIndex]->getSpeed());
   g_ttf_text_renderer->drawText(text, -0.95, 0.8, 2.0/g_current_width, 2.0/g_current_height);

   // draw squashes
   sprintf(text, "points: %d", kart_objects[kartIndex]->getPoints());
   g_ttf_text_renderer->drawText(text, 0.2, 0.8, 2.0/g_current_width, 2.0/g_current_height);
   
   // draw fps
   sprintf(text, "fps: %.0f", 1/dt);
   g_ttf_text_renderer->drawText(text, 0.4, 0.6, 2.0/g_current_width, 2.0/g_current_height);
   
   // draw height
   sprintf(text, "height: %.1f", kart_objects[kartIndex]->getPosition().y-kart_objects[0]->getRideHeight());
   g_ttf_text_renderer->drawText(text, -0.95, 0.6, 2.0/g_current_width, 2.0/g_current_height);


   
   
}

void drawHUD (int kartIndex)
{
   glDisable(GL_DEPTH_TEST);
   glAlphaFunc(GL_GREATER,0.1f);
   glEnable(GL_ALPHA_TEST);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glEnable(GL_BLEND);
   
   setOrthographicMatrix();
   setHUDView();
   
   meshShader->use();
   meshShader->setProjMatrix(g_proj);
   meshShader->setViewMatrix(g_view);
   
   GLuint textureId;
   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, g_current_width, g_current_height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glBindTexture(GL_TEXTURE_2D, 0);
   
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D, textureId, 0);
   
   glClearColor (0.5f, 0.7f, 0.1f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
   glClearColor (1.0f, 1.0f, 0.1f, 0.0f);
   //glClear(GL_COLOR_BUFFER_BIT);
   
   g_model_trans.pushMatrix();
   g_model_trans.loadIdentity();
   
   GameHUD *hud = new GameHUD(g_current_width, g_current_height);
   hud->drawSpeed(meshShader, g_model_trans, kart_objects[kartIndex]->getSpeed());
   
   g_model_trans.popMatrix();
   
   glDeleteTextures(1, &textureId);
   
   //glDisable(GL_BLEND);
   glDisable(GL_ALPHA_TEST);
   glEnable(GL_DEPTH_TEST);
   
}

void drawMultipleViews(double dt, int numViews)
{
   if (numViews == 1) {
      g_current_height = g_win_height;
      g_current_width = g_win_width;
   } else if (numViews == 2) {
      g_current_height = g_win_height/2;
      g_current_width = g_win_width;
   } else if (numViews == 3 || numViews == 4) {
      g_current_height = g_win_height/2;
      g_current_width = g_win_width/2;
   }
   
   int kartIndex = 0;
   for (int i = 0; i * g_current_height < g_win_height; i++) {
      for (int j = 0; j * g_current_width < g_win_width; j++) {
         glViewport( j * g_current_width, i * g_current_height, g_current_width, g_current_height );
         glScissor( j * g_current_width, i * g_current_height, g_current_width, g_current_height );
         
         if (kartIndex < (int)kart_objects.size()) {
            draw(dt, kartIndex);
if(motionBlur == 1)
{glAccum(GL_MULT, .9);
glAccum(GL_ACCUM, 1-.9);
glAccum(GL_RETURN, 1.0);
glFlush();
}
            drawHUD(kartIndex);
            kartIndex++;
               glfwSwapBuffers();
         } else {
            glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
         }
      }
   }
}

void gameLoop()
{
   double dt;

   g_time = glfwGetTime();

   dt = g_time - g_last_time;

   update(dt);
   
   drawMultipleViews(dt, kart_objects.size());
   


   g_last_time = g_time;   	
}




void initObjects() {
   cout << "Initializing game objects\n";
   
   // TODO - test for return values (but we usually know if they work or not)
   g_camera = new GameCamera();
   
   meshShader = new PhongShader();
   // Light 
   g_lightInfo.pos = vec3(1, 15, 1);
   g_lightInfo.color = vec3(1.0f, 1.0f, 1.0f); 

   meshShader->setLight(g_lightInfo);
   meshShader->setShowNormals(0);


   // Bunnie
   GameTerrain *floor = new GameTerrain();
   floor->setScale(vec3(100.0, 0.9, 100.0));
   floor->setPosition(vec3(0, 0, 0));
   drawable_objects.push_back(floor);
   
   for (int i = 0; i < 20; i++) {
      GameDrawableObject *object = new GameDrawableObject("cube");
      object->setName("thingy");
      object->setPosition(vec3(i + 20*randFloat(), 1.0, 2*i +
               30*randFloat()));
      object->setScale(vec3(0.1, 0.5, 0.1));
      drawable_objects.push_back(object);
   }
   

   int num_players_from_settings = g_settings["num_players"];

   if (num_players_from_settings > 0) {
      g_num_players = num_players_from_settings;
   }

   // hax
   // 1st kart
   if (g_num_players >= 1) {
      GameKartObject *kart = new GameKartObject("cube");
      kart->setPosition(vec3(30, 1, 30));
      kart->setScale(vec3(1.0, 0.75, 1.0));
      kart->setDirection(180);
      kart->setInputMap('W', 'S', 'A', 'D', ' ');
      drawable_objects.push_back(kart);
      kart_objects.push_back(kart);
   }
   if (g_num_players >= 2) {
      GameKartObject *otherKart = new GameKartObject("cube");
      otherKart->setPosition(vec3(45, 1, 30));
      otherKart->setScale(vec3(1.0, 0.75, 1.0));
      otherKart->setDirection(0);
      otherKart->setInputMap(GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_ENTER);
      drawable_objects.push_back(otherKart);
      kart_objects.push_back(otherKart);
   }
   if (g_num_players >= 3) {
      GameKartObject *thirdKart = new GameKartObject("cube");
      thirdKart->setPosition(vec3(30, 1, 45));
      thirdKart->setScale(vec3(1.0, 0.75, 1.0));
      thirdKart->setDirection(0);
      drawable_objects.push_back(thirdKart);
      kart_objects.push_back(thirdKart); 
   }

   //upgrades
   GamePartUpgrade *part = new GamePartWings();
   part->setPosition(vec3(5, 1, 2));
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
   
   
   GameRamp *ramp = new GameRamp();
   ramp->setPosition(vec3(-25, 2, -25));
   ramp->setScale(vec3(3.0, 2.0, 3.0));
   drawable_objects.push_back(ramp);
   
   GameBuilding *buildin = new GameBuilding();
   buildin->setPosition(vec3(-25, 2, 0));
   buildin->setScale(vec3(10.0, 2.0, 10.0));
   drawable_objects.push_back(buildin);
   
   glGenFramebuffers(1, &fbo);
   
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


void initMap(const char *filename) {

   loadMap(filename, drawable_objects);


}


void initialize()
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

   initObjects();
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

   initialize();

   if (argc == 2) {
      initMap(argv[1]);
   }

   glfwSetTime(0.0);
   g_last_time = glfwGetTime();

   while (glfwGetWindowParam(GLFW_OPENED)) {
      gameLoop();
   }

   glfwTerminate();
   return 0;
}


// vim modeline
// vim: set sw=3 ts=3 :

