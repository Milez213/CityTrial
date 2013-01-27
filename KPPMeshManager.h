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



struct mesh {
	string name;
	
	vector<GLuint> vertexBuffer;
	vector<GLuint> normalBuffer;
	vector<GLuint> indexBuffer;
	vector<GLuint> textureBuffer;
	vector<GLuint> textureImageBuffer;
	vector<int> indexBufferLength;
	
	vector<vec3> diffuseColor;
	vector<float> specularity;
};

struct bound {
	vec3 boundingBoxMin;
	vec3 boundingBoxMax;
	vec3 center;
	float radius;
};

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
	// of a mesh.
	// 
	// If a mesh is already loaded, it will return
	// the loaded mesh.
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
