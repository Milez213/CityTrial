//
//  test.cpp
//  
//
//  Created by Bryan Bell on 1/29/13.
//
//

#include "GL/glfw.h"

#include "ModelManager.h"
#include "GameDrawableObject.h"

ModelManager *g_model_manager;

int main(int argc, char** argv)
{
   g_model_manager = new ModelManager();
   
   if (!glfwInit()) {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      exit( EXIT_FAILURE );
   }
   
   if (!glfwOpenWindow(640, 480, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
      fprintf( stderr, "Failed to open GLFW window\n" );
      glfwTerminate();
      exit( EXIT_FAILURE );
   } else {
      int major, minor, rev;
		glfwGetGLVersion(&major, &minor, &rev);
 		fprintf(stderr, "OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
	}
   
   GameDrawableObject *object = new GameDrawableObject("Hello");
}
