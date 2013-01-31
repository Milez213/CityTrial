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

#include "ModelManager.h"

ModelManager::ModelManager()
{
   
}

ModelManager::~ModelManager()
{
   for (int i = 0; i < (int)storage.size(); i++) {
      delete[] storage[i].indexBuffer;
      delete[] storage[i].indexBufferLength;
      delete[] storage[i].diffuseColor;
      delete[] storage[i].specularity;
   }
}

bool ModelManager::getObject(const char *fileName, bufferStore *meshes)
{
   int index = -1, test = 0;

   while (index == -1 && test < (int)storage.size()) {
      string file(fileName);
      if (file == storage[test].name) {
         index = test;
      }
      test++;
   }
      
   if (index == -1) {
      loadObject(fileName);
      index = (int)storage.size() - 1;
   }
   	
   meshes->vertexBuffer = storage[index].vertexBuffer;
   meshes->normalBuffer = storage[index].normalBuffer;
   meshes->textureBuffer = storage[index].textureBuffer;
   
   meshes->indexBuffer = new GLuint[storage[index].numMeshes];
   meshes->indexBufferLength = new int[storage[index].numMeshes];
   meshes->diffuseColor = new vec3[storage[index].numMeshes];
   meshes->specularity = new float[storage[index].numMeshes];
   
   meshes->numMeshes = storage[index].numMeshes;
   
   for (int i = 0; i < meshes->numMeshes; i++) {
      meshes->indexBuffer[i] = storage[index].indexBuffer[i];
      meshes->indexBufferLength[i] = storage[index].indexBufferLength[i];
      meshes->diffuseColor[i] = storage[index].diffuseColor[i];
      meshes->specularity[i] = storage[index].specularity[i];
   }

#ifdef DEBUG_VBO   
   printf("VBO Transfered to Given Pointer Location: %d\n", (int)*vertexBuffer);
#endif
   return true;
}

void ModelManager::loadObject(const char *filename)
{
   string file(filename);
   if (file == "ramp") {
      storage.push_back(rampMesh());
   } else if (file == "floor") {
      storage.push_back(rampMesh());
   } else {
      storage.push_back(cubeMesh());
   }
#ifdef DEBUG_VBO
   printf("VBO Transfered to Object Memory: %d\n", (int)storage[0].vertexBuffer);
#endif
}

bufferStore ModelManager::cubeMesh()
{
	bufferStore store;
	
   float verticies[72] = {
		-1.0, -1.0, -1.0,
      -1.0, -1.0, -1.0,
      -1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,
      -1.0, 1.0, -1.0,
      -1.0, 1.0, -1.0,
		1.0, 1.0, -1.0,
      1.0, 1.0, -1.0,
      1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,
      -1.0, 1.0, 1.0,
      -1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
      1.0, 1.0, 1.0,
      1.0, 1.0, 1.0,
		1.0, -1.0, 1.0,
      1.0, -1.0, 1.0,
      1.0, -1.0, 1.0
   };
   	
   float normals[72] = {
		0.0, 0.0, -1.0,
      -1.0, 0.0, 0.0,
      0.0, -1.0, 0.0,
		0.0, 0.0, -1.0, //3
      -1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 0.0, -1.0, //6
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 0.0, -1.0, //9
      1.0, 0.0, 0.0,
      0.0, -1.0, 0.0,
		0.0, 0.0, 1.0,  //12
      -1.0, 0.0, 0.0,
      0.0, -1.0, 0.0,
		0.0, 0.0, 1.0,  //15
      -1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,  //18
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,  //21
      -1.0, 0.0, 0.0,
      0.0, -1.0, 0.0
   };
   	
   /*GLuint faces[36] = {
		1, 3, 0,
		1, 2, 3,
		5, 1, 0,
		5, 0, 4,
		6, 2, 3,
		6, 3, 7,
		5, 6, 7, 
		5, 7, 4,
		1, 2, 6,
		1, 6, 5,
		0, 3, 7,
		0, 7, 4
   	};*/
   
   GLushort front[6] = {
		0, 6, 3,
		0, 6, 9
   };
   GLushort back[6] = {
		12, 18, 15,
		12, 18, 21
   };
   GLushort left[6] = {
		4, 13, 16,
		4, 13, 1
   };
   GLushort right[6] = {
		7, 22, 19,
		7, 22, 10
   };
   GLushort top[6] = {
		5, 20, 17,
		5, 20, 8
   };
   GLushort bottom[6] = {
		2, 23, 14,
		2, 23, 11
   };
   
   GLuint vbo, ibo, ibo2, ibo3, ibo4, ibo5, ibo6, nbo;

   int ibl = 6;
#ifdef DEBUG_VBO   
   printf("VBO Pre-Initialization: %d\n", (int)vbo);
#endif   
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(front), front, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo2);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(back), back, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo3);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(left), left, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo4);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo4);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(right), right, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo5);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo5);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo6);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo6);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bottom), bottom, GL_STATIC_DRAW);
   
   glGenBuffers(1, &nbo);
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
#ifdef DEBUG_VBO   
   printf("VBO Post-Initialization: %d\n", (int)vbo);
#endif   
   store.vertexBuffer = vbo;
   store.normalBuffer = nbo;
   store.textureBuffer = 0;
   
   store.indexBuffer = new GLuint[6];
   store.indexBufferLength = new int[6];
   store.diffuseColor = new vec3[6];
   store.specularity = new float[6];
   
   store.numMeshes = 6;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.diffuseColor[0] = vec3(0.5, 0.0, 0.0);
   store.specularity[0] = 0.4;
   
   store.indexBuffer[1] = ibo2;
   store.indexBufferLength[1] = ibl;
   store.diffuseColor[1] = vec3(0.5, 0.0, 0.0);
   store.specularity[1] = 0.4;
   
   store.indexBuffer[2] = ibo3;
   store.indexBufferLength[2] = ibl;
   store.diffuseColor[2] = vec3(0.0, 0.5, 0.0);
   store.specularity[2] = 0.4;
   
   store.indexBuffer[3] = ibo4;
   store.indexBufferLength[3] = ibl;
   store.diffuseColor[3] = vec3(0.0, 0.5, 0.0);
   store.specularity[3] = 0.4;
   
   store.indexBuffer[4] = ibo5;
   store.indexBufferLength[4] = ibl;
   store.diffuseColor[4] = vec3(0.0, 0.0, 0.5);
   store.specularity[4] = 0.4;
   
   store.indexBuffer[5] = ibo6;
   store.indexBufferLength[5] = ibl;
   store.diffuseColor[5] = vec3(0.0, 0.0, 0.5);
   store.specularity[5] = 0.4;
   
   store.name = "cube";
   
#ifdef DEBUG_VBO   
   printf("VBO Transfered to Local Variable \"store\": %d\n", (int)store.indexBufferLength[0]);
#endif   
   return store;
}

bufferStore ModelManager::rampMesh()
{
	bufferStore store;
	
   float verticies[54] = {
		-1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,  //3
      1.0, -1.0, 1.0,
      1.0, -1.0, 1.0,
		-1.0, -1.0, -1.0,//6
      -1.0, -1.0, -1.0,
      -1.0, -1.0, -1.0,
		1.0, -1.0, -1.0, //9
      1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
		-1.0, 1.0, 1.0,  //12
      -1.0, 1.0, 1.0,
      -1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,   //15
      1.0, 1.0, 1.0,
      1.0, 1.0, 1.0
   };
   
   float normals[54] = {
		0.0, 0.0, -1.0,
      -1.0, 0.0, 0.0,
      0.0, -1.0, 0.0,
		0.0, 0.0, -1.0, //3
      -1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 0.0, -1.0, //6
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 0.0, -1.0, //9
      1.0, 0.0, 0.0,
      0.0, -1.0, 0.0,
		0.0, 0.0, 1.0,  //12
      -1.0, 0.0, 0.0,
      0.0, -1.0, 0.0,
		0.0, 0.0, 1.0,  //15
      -1.0, 0.0, 0.0,
      0.0, 1.0, 0.0
   };
   
   /*GLuint faces[36] = {
    1, 3, 0,
    1, 2, 3,
    5, 1, 0,
    5, 0, 4,
    6, 2, 3,
    6, 3, 7,
    5, 6, 7,
    5, 7, 4,
    1, 2, 6,
    1, 6, 5,
    0, 3, 7,
    0, 7, 4
    };*/
   
   GLushort back[6] = {
		0, 15, 12,
		0, 15, 3
   };
   GLushort left[3] = {
		6, 1, 13
   };
   GLushort right[3] = {
		9, 4, 16
   };
   GLushort top[6] = {
		7, 17, 14,
		7, 17, 10
   };
   GLushort bottom[6] = {
		8, 5, 2,
		8, 5, 11
   };
   
   GLuint vbo, ibo, ibo2, ibo3, ibo4, ibo5, nbo;
   
   int ibl = 6, ibl2 = 3;
#ifdef DEBUG_VBO
   printf("VBO Pre-Initialization: %d\n", (int)vbo);
#endif
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bottom), bottom, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo2);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(back), back, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo3);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(left), left, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo4);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo4);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(right), right, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo5);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo5);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(top), top, GL_STATIC_DRAW);
   
   glGenBuffers(1, &nbo);
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
#ifdef DEBUG_VBO
   printf("VBO Post-Initialization: %d\n", (int)vbo);
#endif
   store.vertexBuffer = vbo;
   store.normalBuffer = nbo;
   store.textureBuffer = 0;
   
   store.indexBuffer = new GLuint[5];
   store.indexBufferLength = new int[5];
   store.diffuseColor = new vec3[5];
   store.specularity = new float[5];
   
   store.numMeshes = 5;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.diffuseColor[0] = vec3(0.5, 0.0, 0.0);
   store.specularity[0] = 0.4;
   
   store.indexBuffer[1] = ibo2;
   store.indexBufferLength[1] = ibl;
   store.diffuseColor[1] = vec3(0.5, 0.0, 0.0);
   store.specularity[1] = 0.4;
   
   store.indexBuffer[2] = ibo3;
   store.indexBufferLength[2] = ibl2;
   store.diffuseColor[2] = vec3(0.0, 0.5, 0.0);
   store.specularity[2] = 0.4;
   
   store.indexBuffer[3] = ibo4;
   store.indexBufferLength[3] = ibl2;
   store.diffuseColor[3] = vec3(0.0, 0.5, 0.0);
   store.specularity[3] = 0.4;
   
   store.indexBuffer[4] = ibo5;
   store.indexBufferLength[4] = ibl;
   store.diffuseColor[4] = vec3(0.0, 0.0, 0.5);
   store.specularity[4] = 0.4;
   
   store.name = "ramp";
   
#ifdef DEBUG_VBO
   printf("VBO Transfered to Local Variable \"store\": %d\n", (int)store.indexBufferLength[0]);
#endif
   return store;
}

bufferStore ModelManager::floorMesh()
{
	bufferStore store;
	
   float verticies[12] = {
		-1.0, 0.0, -1.0,
      -1.0, 0.0, 1.0,
      1.0, 0.0, -1.0,
		1.0, 0.0, 1.0
   };
   
   float normals[12] = {
		0.0, 1.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 1.0, 0.0,
		0.0, 1.0, 0.0
   };
   
   /*GLuint faces[36] = {
    1, 3, 0,
    1, 2, 3,
    5, 1, 0,
    5, 0, 4,
    6, 2, 3,
    6, 3, 7,
    5, 6, 7,
    5, 7, 4,
    1, 2, 6,
    1, 6, 5,
    0, 3, 7,
    0, 7, 4
    };*/
   
   GLushort floor[6] = {
		0, 3, 1,
		0, 3, 2
   };
   
   GLuint vbo, ibo, nbo;
   
   int ibl = 6;
#ifdef DEBUG_VBO
   printf("VBO Pre-Initialization: %d\n", (int)vbo);
#endif
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
   
   glGenBuffers(1, &nbo);
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
#ifdef DEBUG_VBO
   printf("VBO Post-Initialization: %d\n", (int)vbo);
#endif
   store.vertexBuffer = vbo;
   store.normalBuffer = nbo;
   store.textureBuffer = 0;
   
   store.indexBuffer = new GLuint[1];
   store.indexBufferLength = new int[1];
   store.diffuseColor = new vec3[1];
   store.specularity = new float[1];
   
   store.numMeshes = 1;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.diffuseColor[0] = vec3(0.5, 0.0, 0.0);
   store.specularity[0] = 0.4;
   
   store.name = "floor";
   
#ifdef DEBUG_VBO
   printf("VBO Transfered to Local Variable \"store\": %d\n", (int)store.indexBufferLength[0]);
#endif
   return store;
}