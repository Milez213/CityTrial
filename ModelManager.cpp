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
   if (storage.empty()) {
      loadObject("Hello!");
   }
   	
   meshes->vertexBuffer = storage[0].vertexBuffer;
   meshes->normalBuffer = storage[0].normalBuffer;
   meshes->textureBuffer = storage[0].textureBuffer;
   
   meshes->indexBuffer = new GLuint[storage[0].numMeshes];
   meshes->indexBufferLength = new int[storage[0].numMeshes];
   meshes->diffuseColor = new vec3[storage[0].numMeshes];
   meshes->specularity = new float[storage[0].numMeshes];
   
   meshes->numMeshes = storage[0].numMeshes;
   
   for (int i = 0; i < meshes->numMeshes; i++) {
      meshes->indexBuffer[i] = storage[0].indexBuffer[i];
      meshes->indexBufferLength[i] = storage[0].indexBufferLength[i];
      meshes->diffuseColor[i] = storage[0].diffuseColor[i];
      meshes->specularity[i] = storage[0].specularity[i];
   }

#ifdef DEBUG_VBO   
   printf("VBO Transfered to Given Pointer Location: %d\n", (int)*vertexBuffer);
#endif
   return true;
}

void ModelManager::loadObject(const char *filename)
{
	storage.push_back(cubeMesh());
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
   
#ifdef DEBUG_VBO   
   printf("VBO Transfered to Local Variable \"store\": %d\n", (int)store.indexBufferLength[0]);
#endif   
   return store;
}