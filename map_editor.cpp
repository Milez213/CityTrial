//
// map_editor.cpp
//
//
// Created by Mustafa Khafateh
//
// Map editor!
//
//
//
// 
// Date Created: 2/10/2013
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
#include "GameCameraFirstPerson.h"
#include "GameRamp.h"
#include "GameBuilding.h"
#include "GameTerrain.h"

#include "GamePartWings.h"
#include "GameStatSpeed.h"

#ifdef MAIN_USE_TTF
#include "TTFRenderer.h"
#else
#include "DummyTextRenderer.h"
#endif

#include "loadMap.h"

#include "GameSettings.h"

using namespace std;


// === Globals =================================

// Singletons
// There should be only one instance of these
ModelManager *g_model_manager;
SoundManager *g_sound_manager;
TextRenderer *g_ttf_text_renderer;

GameSettings g_settings;


// test one object for now
PhongShader *meshShader;
vector<GameDrawableObject *> drawable_objects;
vector<GameKartObject *> kart_objects;


RenderingHelper g_model_trans;


// GLFW Window
int g_win_height, g_win_width;
int g_current_height, g_current_width;

double g_time;
double g_last_time;

mat4 g_proj;
mat4 g_view;
mat4 g_model;

GameCameraFirstPerson *g_camera;

glm::vec2 g_mousePos;

// which direction player is moving?
vec3 g_playerDir;
float g_playerMoveSpeed = 4;


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

void setProjectionMatrix() {
   g_proj = glm::perspective( 100.0f,
         (float)g_win_width/g_win_height, 0.1f, 100.f);
}

void setOrthographicMatrix() {
   g_proj = glm::ortho(0.0f, (float)g_win_width, (float)g_win_height, 0.0f, -1.0f, 1.0f);

}


/* camera controls */
// 3rd person view
void setView() {
   g_view = g_camera->getViewMat();
   
}



/* TODO - put in keyboard callback
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
      }
      
      //printf("Joy: %0.3f\n", joy[3]);
      kart_objects[i]->setJoystickState(joy); //These functions are commented out in GameKartObject *****
      //kart_objects[i]->setButtonState(button);//Update internal input arrays in kartObject, then allow it to update based on given input *****
   }
}
*/

void rotateCamera() {
   g_camera->angle.x += 50 * g_mousePos.x;
   g_camera->angle.y -= 50 * g_mousePos.y;
}


void update(double dt)
{

	//ExtractFrustum();
   
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      drawable_objects[i]->update(dt);
   }
   
   g_camera->update(dt, g_playerDir, g_playerMoveSpeed);

}



void draw()
{
   glClearColor (0.8f, 0.8f, 1.0f, 1.0f);
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
   setProjectionMatrix();
   setView();

   // set once for this shader
   meshShader->use();
   meshShader->setProjMatrix(g_proj);
   meshShader->setViewMatrix(g_view);
  

   // get camera position
   // TODO - fix
   /*
   vec3 kartPos = kart_objects[kartIndex]->getPosition();
   vec3 kartDir = normalize(kart_objects[kartIndex]->getDirectionVector());
   kartDir = vec3(kartDir.x * 3.0, kartDir.y * 3.0 - 2.0, kartDir.z * 3.0);
   */

   // meshShader->setCamPos(kartPos - kartDir);


   // draw objects
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      setPhongMaterial(i%NUM_MATERIALS);
      drawable_objects[i]->draw(meshShader, g_model_trans);
   }



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
   
   g_camera = new GameCameraFirstPerson();
   g_camera->setPosition(vec3(0, 5, 0));
   
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
   
   
   /* Example
   // Example
   GamePartUpgrade *part = new GamePartWings();
   part->setPosition(vec3(5, 1, 2));
   part->setScale(vec3(2.0, 1.0, 1.0));
   drawable_objects.push_back(part);
   */

}


void initMap(const char *filename) {

   // loadMap(filename, drawable_objects);


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
#else

   g_sound_manager = new SDLSoundManager();

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


int isPressed(int action) {
   return action == GLFW_PRESS ? 1 : 0;
}


// Keyboard callback
void GLFWCALL keyboard_callback_key(int key, int action) {
   // only gives upercase
   switch (key) {
   case 'W':
      if (isPressed(action)) {
         g_playerDir.z = -1;
      } else {
         g_playerDir.z = 0;
      }
      break;

   case 'A':
      if (isPressed(action)) {
         g_playerDir.x = -1;
      } else {
         g_playerDir.x = 0;
      }
      break;

   case 'S':
      if (isPressed(action)) {
         g_playerDir.z = 1;
      } else {
         g_playerDir.z = 0;
      }
      break;

   case 'D':
      if (isPressed(action)) {
         g_playerDir.x = 1;
      } else {
         g_playerDir.x = 0;
      }
      break;

   case 'Q':
   case GLFW_KEY_ESC:
      printf("Bye :)\n");
      shutdown();
      break;
   }
}



// pixel coords to window coords
float p2wx(int in_x) {
  //fill in with the correct return value
  return ((2.0 / (g_win_width - 1)) * in_x) - 1;
}


float p2wy(int in_y) {
  // flip glut's y
  in_y = g_win_height - in_y - 1;

  // fill in with the correct return value
  return ((2.0 / (g_win_height - 1)) * in_y) - 1;;
}


void GLFWCALL mouseMove(int x, int y) {

   g_mousePos.y = p2wx(x);
   g_mousePos.x = p2wy(y);

   if (g_camera) 
      rotateCamera();
   
   glfwSetMousePos(g_win_width/2, g_win_height/2);
   
   printf("x: %f, y: %f\n", g_mousePos.x, g_mousePos.y);

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
   		int major, minor, rev;
		glfwGetGLVersion(&major, &minor, &rev);
 		fprintf(stderr, "OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	}

   glfwSetWindowSizeCallback( reshape );
   glfwSetKeyCallback( keyboard_callback_key );
   glfwSetMousePosCallback( mouseMove );

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

