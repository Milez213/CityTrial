//
// KPPMeshLoader.cpp
// Final Project - Kart Part Park
// 
// Created by Bryan Bell
// Date Created: 1/25/13
//
// The KPPMeshLoader class is directly responsible
//  for managing meshes needed in runtime for 
//  drawing.
//
// For CSC476 - Real Time 3D Rendering

typedef struct material {
	vec3 diffuseColor;
	float specularity;
} material;

typedef struct mesh {
	string name;
	
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint textureBuffer;
	GLuint textureImageBuffer;
	int indexBufferLength;
} mesh;

typedef struct bound {
	vec3 boundingBoxMin;
	vec3 boundingBoxMax;
	vec3 center;
	float radius;
} bound;

class KPPMeshLoader {
public:
	//--------------------------------------------
	// Initialized with a reference to the shader
	//  used when drawing a mesh selected by 
	//  indexing the mesh array.
	//--------------------------------------------
	KPPMeshLoader(KPPShader *shd, RenderingHelper *model);
	
	//--------------------------------------------
	// This function takes in the name of a file 
	//  as well as storage locations for the indicies
	//  of the created mesh(es) and an array of 
	//  created materials which are associated with
	//  the given mesh(es)
	// 
	// Parameters:
	// filename - name of the .obj file to load
	// indices - storage location for indices of 
	//  created mesh(es) to be stored into.
	// mtl - storage location of material proporties
	//  for associated mesh(es).
	// bnd - storage location of bounding box info
	//  (This will be the averaged information of
	//  all the meshes found in the file)
	//
	// Returns:
	// int - number of meshes loaded from .obj file
	//--------------------------------------------
	int loadMesh(const char* filename, int *indices, material *mtl, bound *bnd);	
	
	//--------------------------------------------
	// This function draws a mesh given by its
	//  associated index in KPPMeshLoader's
	//  internal mesh storage.
	//
	// Parameters:
	// ind - index of mesh to draw. (stored into 
	//  indices by "loadMesh")
	//-------------------------------------------- 
	void drawMesh(int ind);
	
private:
	KPPShader *shader;
	RenderingHelper *model_trans;

	vector<mesh> meshes;
}