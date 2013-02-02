/*
 * ModelManager.h
 * Final Project - Kart Part Park
 *
 * Created by Bryan Bell
 * Date Created: 1/25/13
 *
 * The ModelManager class is directly responsible
 *  for managing meshes needed in runtime for
 *  drawing.
 */

#ifndef MODEL_MANAGER
#define MODEL_MANAGER

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#include <string>
#include <vector>
#include "glm/glm.hpp"

using namespace glm;
using namespace std;

struct bufferStore {
	string name;
	
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint textureBuffer;   
	GLuint *indexBuffer;
	int *indexBufferLength;
	
	vec3 *diffuseColor;
	float *specularity;
   
   int numMeshes;
};
 
struct bound {
   vec3 bottomLeft;
   vec3 dimension;
   vec3 center;
   float radius;
   
   bool sphereOnSphere(bound objOne, bound objTwo);
   bool sphereOnBox(bound objOne, bound objTwo);
   bool boxOnBox(bound objOne, bound objTwo);
};

bool sphereOnSphere(bound objOne, bound objTwo)
{
   vec3 diff = objOne.center - objTwo.center;
   float dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2) + pow(diff.z, 2));
   
   if (dist < objOne.radius + objTwo.radius) {
      return true;
   }
   
   return false;
}

bool sphereOnBox(bound objOne, bound objTwo)
{
   return false;
}

bool boxOnBox(bound objOne, bound objTwo)
{
   if (objOne.bottomLeft.x > objTwo.bottomLeft.x + objTwo.dimension.x ||
       objOne.bottomLeft.x + objOne.dimesnion.x < objTwo.dimension.x  ||
       objOne.bottomLeft.y > objTwo.bottomLeft.y + objTwo.dimension.y ||
       objOne.bottomLeft.y + objOne.dimesnion.y < objTwo.dimension.y  ||
       objOne.bottomLeft.z > objTwo.bottomLeft.z + objTwo.dimension.z ||
       objOne.bottomLeft.z + objOne.dimesnion.z < objTwo.dimension.z) {
      return false;
   }
   
   return true;
}

class ModelManager {

public:
	//--------------------------------------------
	// Initialized with a reference to the shader
	//  used when drawing a mesh selected by 
	//  indexing the mesh array.
	//--------------------------------------------
	ModelManager();
   ~ModelManager();
	
	//--------------------------------------------
	// This function takes in the name of a file
	// of a mesh.
	// 
	// If a mesh is already loaded, it will return
	// the loaded mesh.
	// 
	// Parameters:
	// filename - name of the .obj file to load
   // vertexBuffer - vertexBuffer to fill
   // textureBuffer - textureBuffer to fill
   // normalBuffer - normalBuffer to fill
   // indexBuffer - indexBuffer to fill
   // indexBufferLength - indexBufferLength to fill
	//
	// Returns:
	// true if successfull, false if unsuccessful
	//--------------------------------------------
	bool getObject(const char *fileName, bufferStore *meshes, bound *boundingInfo);
	
private:
	void loadObject(const char* filename);
   bufferStore cubeMesh();
   bufferStore rampMesh();
   bufferStore floorMesh();
	vector<bufferStore> storage;
};

#endif