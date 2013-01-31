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

#include "FlatShader.h"
#include "ModelManager.h"
#include "GamePhysics.h"

#include "GameDrawableObject.h"
#include "GameUpgradeObject.h"
#include "GameKartObject.h"

#include "GameUtilities.h"

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

/*
// TODO
KPPPhysics *physics_sim;
KPPMeshManager *mesh_manager;
KPPShader *shader;

vector<KPPObject *> moving_objects;
vector<KPPDrawnObject *> drawn_objects;
vector<KPPKartObject *> kart_objects;
*/

ModelManager *g_model_manager;
GamePhysics *g_physics;

// test one object for now
FlatShader *flatShader;
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
vec3 g_lookAt;



// === end Globals ==============================


/* projection matrix */
void setProjectionMatrix() {
   g_proj = glm::perspective(90.0f, (float)g_win_width/g_win_height, 0.1f, 100.f);
}


/* camera controls */
void setView() {
   vec3 up = glm::vec3(0.0, 1.0, 0.0);
   vec3 kartPos = kart_objects[0]->position();
   glm::mat4 lookAt = glm::lookAt(vec3(kartPos.x, kartPos.y + 2.0, kartPos.z - 3.0), kartPos, up);
   g_view = lookAt;
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
            joy[0] = 1.0;
         else if (glfwGetKey('S') == GLFW_PRESS)
            joy[0] = -1.0;
         else
            joy[0] = 0.0;
         if (glfwGetKey('A') == GLFW_PRESS)
            joy[3] = 1.0;
         else if (glfwGetKey('D') == GLFW_PRESS)
            joy[3] = -1.0;
         else
            joy[3] = 0.0;
      }
      
      //printf("Joy: %0.3f\n", joy[3]);
      kart_objects[i]->setJoystickState(joy); //These functions are commented out in GameKartObject *****
      //kart_objects[i]->setButtonState(button);//Update internal input arrays in kartObject, then allow it to update based on given input *****
   }
}


void update(double dt)
{
   for (int i = 0; i < (int)kart_objects.size(); i++) {
      kart_objects[i]->update(dt);
   }

   getInputState();
   
   wings->update(g_time, dt);
   
   g_physics->simulate(dt);
   
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
   flatShader->use();
   flatShader->setProjMatrix(g_proj);
   flatShader->setViewMatrix(g_view);
   
   for (int i = 0; i < (int)drawable_objects.size(); i++) {
      drawable_objects[i]->draw(flatShader, g_model_trans);
   }


   /* psuedocode
   for each (KKPDrawnObject object in drawn_objects) {
      object->draw(model_trans);
   }
   */

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

   flatShader = new FlatShader();

   // Bunnie
   GamePhysicsActor *fActor = g_physics->makeStaticActor(physx::PxTransform(physx::PxVec3(0.0, -0.1, 0.0)), new physx::PxBoxGeometry(convert(glm::vec3(25.0, 0.1, 25.0))), g_physics->makeMaterial());
   GameDrawableObject *floor = new GameDrawableObject(fActor, "floor");
   floor->setScale(vec3(25.0, 0.0, 25.0));
   drawable_objects.push_back(floor);
   
   for (int i = -10; i < 11; i++) {
      for (int j = 5; j < 11; j++) {
         GamePhysicsActor *actor = g_physics->makeStaticActor(physx::PxTransform(physx::PxVec3(0.0)), new physx::PxBoxGeometry(convert(glm::vec3(0.1, 0.1, 0.1))), g_physics->makeMaterial());
         GameDrawableObject *object = new GameDrawableObject(actor, "cube");
         object->setPosition(vec3(i, 1.0, j));
         object->setScale(vec3(0.1, 0.1, 0.1));
         drawable_objects.push_back(object);
      }
   }
   
   GameKartObject *kart = new GameKartObject("cube");
   drawable_objects.push_back(kart);
   kart_objects.push_back(kart);
   
   
   wings = new GameUpgradeObject(GameUpgradeObject::FLIGHT, glm::vec3(10.0, 2.0, 10.0));
   drawable_objects.push_back(wings);
   
   /*GameKartObject *kart = new GameKartObject("Kart");
   if (glfwGetJoystickParam(kart_objects.size(), GLFW_PRESENT) == GL_TRUE) { // What code should look like for Kart Objects *****
      printf("Controller Connected for Player %d\n", kart_objects.size());   // Please uncomment and test                   *****
      kart->setUsingController(true);
   }
   kart_objects.push_back(kart);
   drawable_objects.push_back(kart);*/
}




//-----------------------------------------------
// This function initializes geometry from a 
//  geometry file "geometry_loader", which has 
//  has the format:
//  (type) (.obj file) (type specific data) ...
//-----------------------------------------------
/*
void initGeometry()
{
   ifstream in("geometry_loader", ios::in);
   if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

   string line;
   while (getline(in, line)) {
      istringstream s(line);

      char type;
      type << s;

      string meshFile;
      meshFile << s;

      switch (type) {
      case: "d"
         KPPDrawnObject *object = new KPPDrawnObject(meshFile.c_str());
         //Any initialization from type specific data in file
         drawn_objects.push_back(object);
         moving_objects.push_back(object);
         break;
      case: "k"
         KKPKartObject *object = new KPPKartObject(meshFile.c_str());
         if (glfwGetJoystickParam(kart_objects.size(), GLFW_PRESENT) == GL_TRUE) {
            printf("Controller Connected for Player %d\n", kart_objects.size());
         }
         //Any initialization from type specific data in file
         kart_objects.push_back(object);
         drawn_objects.push_back(object);
         moving_objects.push_back(object);
         break;
      default:
         cerr << "ERROR: Inproper Type Declared in Geometry Loader File\n";
         exit(0);
      }
   }
}
*/



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
   g_physics = new GamePhysics();
   // initGeometry();

   initObjects();
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
   case 'Q':
   case GLFW_KEY_ESC:
      printf("Bye :)\n");
      exit(0);
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

   glfwSetTime(0.0);

   while (glfwGetWindowParam(GLFW_OPENED)) {
      gameLoop();
   }

   glfwTerminate();
   return 0;
}


// vim modeline
// vim: set sw=3 ts=3 :

