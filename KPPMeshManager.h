//
// KPPMeshManager.cpp
// Final Project - Kart Part Park
// 
// Created by Bryan Bell
// Date Created: 1/25/13
//
// The KPPMeshManager class is directly responsible
//  for managing meshes needed in runtime for 
//  drawing.
//
// For CSC476 - Real Time 3D Rendering

typedef struct mesh {
	string name;
	
	vector<GLuint> vertexBuffer;
	vector<GLuint> normalBuffer;
	vector<GLuint> indexBuffer;
	vector<GLuint> textureBuffer;
	vector<GLuint> textureImageBuffer;
	vector<int> indexBufferLength;
	
	vector<vec3> diffuseColor;
	vector<float> specularity;
} mesh;

typedef struct bound {
	vec3 boundingBoxMin;
	vec3 boundingBoxMax;
	vec3 center;
	float radius;
} bound;

class KPPMeshManager {
public:
	//--------------------------------------------
	// Initialized with a reference to the shader
	//  used when drawing a mesh selected by 
	//  indexing the mesh array.
	//--------------------------------------------
	KPPMeshManager();
	
	//--------------------------------------------
	// This function takes in the name of a file 
	//  as well as storage locations for the indicies
	//  of the created mesh(es) and an array of 
	//  created materials which are associated with
	//  the given mesh(es)
	// 
	// Parameters:
	// filename - name of the .obj file to load
	//
	// Returns:
	// mesh - mesh loaded from manager
	//--------------------------------------------
	mesh getMesh(const char* filename);
	
private:
	void loadMesh(const char* filename);
	vector<mesh> meshes;
}