//
// main.cpp
// Final Project - Kart Part Park
// 
// Created by Bryan Bell, Eric Johnson, Mustafa, Chirs Pauley
// Date Created: 1/25/13
//
// For CSC476 - Real Time 3D Rendering

#include <GL/glfw.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "GLSL_helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "MStackHelp.h"

vector<KPPObject> moving_objects;
vector<KPPDrawnObject> drawn_objects;
vector<KPPKartObject> kart_objects;

int w_wdth, w_hgt;
double last_time;

void update(double deltaTime)
{

}

void init_geometry()
{
	
}

void draw()
{
	
}

void game_loop(double deltaTime)
{
	update(deltaTime);
	draw();
}

void initialize()
{
	/* Start OpenGL Initialization */
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
   
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();
   /* End OpenGL Initialization */
   
   /* Start Shader Initialization */
   if (!InstallShader(textFileRead((char *)"tex_vert.glsl"), textFileRead((char *)"tex_frag.glsl"))) {
      printf("Error installing shader!\n");
      return;
   }
   /* End Shader Initialization */
   
   init_geometry();
	
	if (glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_PRESENT) == GL_TRUE) {
		printf("Number of Axes: %d\n", glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_AXES));
		printf("Number of Buttons: %d\n", glfwGetJoystickParam(GLFW_JOYSTICK_1, GLFW_BUTTONS));
	}
}

void reshape(int width, int height)
{
	w_wdth = width;
	w_hgt = height;
}

int main(int argc, char** argv) 
{
	w_wdth = 640;
	w_hgt = 480;
	
   if (glfwInit() == GL_TRUE) {
    	glfwOpenWindow(w_wdth, w_hgt, 0, 0, 0, 0, 16, 0, GLFW_WINDOW);
    	glfwSetWindowSizeCallback( reshape );
    }
    	
   initialize();
   
   glfwSetTime(0.0);
    	
   while (glfwGetWindowParam(GLFW_OPENED)) {
   		double time = glfwGetTime();
	
    	game_loop(time - last_time);   	
   }
    
   glfwTerminate();
}