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
#include "GameUpgradeObject.h"
#include "GameKartObject.h"
#include "GameCamera.h"
#include "GameRamp.h"
#include "GameBuilding.h"
#include "GameTerrain.h"


#ifdef MAIN_USE_TTF
#include "TTFRenderer.h"
#else
#include "DummyTextRenderer.h"
#endif

#include "loadMap.h"

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


GameSound *g_music;


// test one object for now
PhongShader *meshShader;
vector<GameDrawableObject *> drawable_objects;
vector<GameKartObject *> kart_objects;

GameUpgradeObject *wings;

RenderingHelper g_model_trans;

// GLFW Window
int g_win_height, g_win_width;

double g_time;
double g_last_time;

mat4 g_proj;
mat4 g_view;
mat4 g_model;

GameCamera *g_camera;


// === game info ===

int g_num_squashes = 0;


// *** lights ***

LightInfo g_lightInfo;

#define NUM_MATERIALS 4

PhongMaterial g_materials[NUM_MATERIALS] = {
                  {vec3(0.2, 0.2, 0.2), // amb
                   vec3(0.7, 0.4, 0.4), // diff
                   vec3(1, 1, 1),       // spec
                   20.0},               // shine

                  {vec3(0.0, 0.0, 0.0),
                   vec3(H2_3f(0xfff852)), //Hex color to rgb
                   vec3(1, 1, 1),
                   10.0},

                  {vec3(0.1, 0.1, 0.3),
                   vec3(0.6, 0.1, 0.1),
                   vec3(0.1, 0.1, 0.7),
                   20.0},
                  {vec3(0, 1, 1),  // for drawing light
                   vec3(0.0),
                   vec3(0),
                   0.0},
};

/* helper function to set up material for shading */

void setPhongMaterial(int i) {
    if ((i >= 0) && i < NUM_MATERIALS) {
        meshShader->setMaterial(g_materials[i]);
    }
}


// *** end lights ***

// === end Globals ==============================


/* projection matrix */
void setProjectionMatrix() {
   g_proj = glm::perspective( (float) kart_objects[0]->getSpeed() * 2.0f + 90.0f,
         (float)g_win_width/g_win_height, 0.1f, 100.f);
}


/* camera controls */
void setView() {
   vec3 kartDir = kart_objects[0]->getDirectionVector();
   vec3 kartPos = kart_objects[0]->getPosition();

   // move camera back and up
   kartDir = vec3(kartDir.x * 3.0, kartDir.y * 3.0 - 2.0, kartDir.z * 3.0);

   g_camera->setLookAtTarget(kart_objects[0]->getPosition());
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
         if (glfwGetKey('W') == GLFW_PRESS)
            joy[3] = 1.0;
         else if (glfwGetKey('S') == GLFW_PRESS)
            joy[3] = -1.0;
         else
            joy[3] = 0.0;
         if (glfwGetKey('A') == GLFW_PRESS)
            joy[0] = 1.0;
         else if (glfwGetKey('D') == GLFW_PRESS)
            joy[0] = -1.0;
         else
            joy[0] = 0.0;
      }
      
      //printf("Joy: %0.3f\n", joy[3]);
      kart_objects[i]->setJoystickState(joy); //These functions are commented out in GameKartObject *****
      //kart_objects[i]->setButtonState(button);//Update internal input arrays in kartObject, then allow it to update based on given input *****
   }
}


void update(double dt)
{
   getInputState();
   
   for (int i = 0; i < (int)kart_objects.size(); i++) {
      kart_objects[i]->update(dt);
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
   
   wings->update(g_time, dt);
   
   
   
   /*for (int i = 0; i < kart_objects.size(); i++) {
      kart_objects[i]->update(dt);                  // What loop for moving karts should look like, please test *****
   }*/
   
}




void draw()
{

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   setProjectionMatrix();
   setView();

   // set once for this shader
   meshShader->use();
   meshShader->setProjMatrix(g_proj);
   meshShader->setViewMatrix(g_view);

   // get camera position
   vec3 kartPos = kart_objects[0]->getPosition();
   vec3 kartDir = normalize(kart_objects[0]->getDirectionVector());
   kartDir = vec3(kartDir.x * 3.0, kartDir.y * 3.0 - 2.0, kartDir.z * 3.0);
   meshShader->setCamPos(kartPos - kartDir);

   // choose from materials
   
      setPhongMaterial(0);

   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      drawable_objects[i]->draw(meshShader, g_model_trans);
      setPhongMaterial(1);
   }


   /* psuedocode
   for each (KKPDrawnObject object in drawn_objects) {
      object->draw(model_trans);
   }
   */

   // draw text
   char text[100];
   sprintf(text, "speed: %.0f", kart_objects[0]->getSpeed());   
   g_ttf_text_renderer->drawText(text, -0.95, 0.8, 2.0/g_win_width, 2.0/g_win_height);

   // draw squashes
   sprintf(text, "squashes: %d", g_num_squashes);
   g_ttf_text_renderer->drawText(text, 0.2, 0.8, 2.0/g_win_width, 2.0/g_win_height);
   

   glfwSwapBuffers();
}




void gameLoop()
{
   double dt;

   g_time = glfwGetTime();

   dt = g_time - g_last_time;

   update(dt);

   draw();

   g_last_time = g_time;   	
}




void initObjects() {
   cout << "Initializing game objects\n";
   
   // TODO - test for return values (but we usually know if they work or not)
   g_camera = new GameCamera();
   
   meshShader = new PhongShader();
   // Light 
   g_lightInfo.pos = vec3(1, 5, 1);
   g_lightInfo.color = vec3(1.0f, 1.0f, 1.0f); 

   meshShader->setLight(g_lightInfo);
   meshShader->setShowNormals(0);


   // Bunnie
   GameTerrain *floor = new GameTerrain();
   floor->setScale(vec3(100.0, 0.9, 100.0));
   floor->setPosition(vec3(0, 0, 0));
   drawable_objects.push_back(floor);
   
   for (int i = -10; i < 11; i++) {
      for (int j = 5; j < 11; j++) {
         GameDrawableObject *object = new GameDrawableObject("cube");
         object->setName("thingy");
         object->setPosition(vec3(2*i, 1.0, 2*j));
         object->setScale(vec3(0.1, 0.5, 0.1));
         drawable_objects.push_back(object);
      }
   }
   
   GameKartObject *kart = new GameKartObject("cube");
   kart->setPosition(vec3(30, 1, 30));
   kart->setScale(vec3(1.0, 0.75, 1.0));
   kart->setDirection(180);
   drawable_objects.push_back(kart);
   kart_objects.push_back(kart);
   
   
   wings = new GameUpgradeObject(GameUpgradeObject::FLIGHT);
   wings->setPosition(vec3(5, 1, 2));
   wings->setScale(vec3(2.0, 1.0, 1.0));
   drawable_objects.push_back(wings);
   
   
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
   g_music = g_sound_manager->getMusic("music/raptor.ogg");   
   g_music->play();
#else
   g_sound_manager = new SDLSoundManager();
   g_music = g_sound_manager->getMusic("music/raptor.ogg");
   g_music->play();
#endif

   // initialize with default font
#ifdef MAIN_USE_TTF
   g_ttf_text_renderer = new TTFRenderer("fonts/DejaVuSans.ttf");
#else
   g_ttf_text_renderer = new DummyTextRenderer("lolololol");
#endif

   

   initObjects();
}



void shutdown() {

   delete g_sound_manager;
   exit(0);
}


// For GLFW
void reshape(int width, int height)
{
   g_win_width = width;
   g_win_height = height;
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

   if (!glfwOpenWindow(g_win_width, g_win_height, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
      fprintf( stderr, "Failed to open GLFW window\n" );
      glfwTerminate();
      exit( EXIT_FAILURE );
   } else {
   		int major, minor, rev;
		glfwGetGLVersion(&major, &minor, &rev);
 		fprintf(stderr, "OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	}

   glfwSetWindowSizeCallback( reshape );
   glfwSetKeyCallback( keyboard_callback_key );

   initialize();

   if (argc == 2) {
      initMap(argv[1]);
   }

   glfwSetTime(0.0);

   while (glfwGetWindowParam(GLFW_OPENED)) {
      gameLoop();
   }

   glfwTerminate();
   return 0;
}


// vim modeline
// vim: set sw=3 ts=3 :

