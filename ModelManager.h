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
	bool getObject(const char *fileName, bufferStore *meshes);
	
private:
	void loadObject(const char* filename);
   bufferStore cubeMesh();
   bufferStore rampMesh();
	vector<bufferStore> storage;
};

#endif