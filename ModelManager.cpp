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

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

bool ModelManager::sphereOnSphere(bound objOne, bound objTwo)
{

   /*vec3 diff = objOne.center - objTwo.center;
   float dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2) + pow(diff.z, 2));
   
   if (dist < objOne.radius + objTwo.radius) {*/
   if (glm::length(objOne.center-objTwo.center) < objOne.radius + objTwo.radius) {
      return true;
   }
   
   return false;
}

bool ModelManager::sphereOnBox(bound objOne, bound objTwo)
{
   return false;
}

bool ModelManager::boxOnBox(bound objOne, bound objTwo)
{
   if (objOne.bottomLeft.x > objTwo.bottomLeft.x + objTwo.dimension.x ||
       objOne.bottomLeft.x + objOne.dimension.x < objTwo.dimension.x  ||
       objOne.bottomLeft.y > objTwo.bottomLeft.y + objTwo.dimension.y ||
       objOne.bottomLeft.y + objOne.dimension.y < objTwo.dimension.y  ||
       objOne.bottomLeft.z > objTwo.bottomLeft.z + objTwo.dimension.z ||
       objOne.bottomLeft.z + objOne.dimension.z < objTwo.dimension.z) {
      return false;
   }
   
   return true;
}


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

bool ModelManager::getObject(const char *fileName, bufferStore *meshes, bound *boundingInfo)
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
      // meshes->diffuseColor[i] = storage[index].diffuseColor[i];
      // meshes->specularity[i] = storage[index].specularity[i];
   }
   
   printf("Mesh Info:\n");
   printf("   Number of Meshes: %d\n", meshes->numMeshes);
   for (int i = 0; i < meshes->numMeshes; i++) {
      printf("      Mesh %d has %d index\n", i, (int)meshes->indexBuffer[i]);
   }
   
   boundingInfo->bottomLeft = vec3(-1.0, -1.0, -1.0);
   boundingInfo->dimension = vec3(2.0, 2.0, 2.0);
   boundingInfo->center = vec3(0.0, 0.0, 0.0);
   boundingInfo->radius = 1.0;

#ifdef DEBUG_VBO   
   printf("VBO Transfered to Given Pointer Location: %d\n", (int)*vertexBuffer);
#endif
   return true;
}

void ModelManager::loadObject(const char *filename)
{
   string file(filename);
   if (file == "ramp") {
      printf("Ramp\n");
      storage.push_back(rampMesh());
   } else if (file == "floor") {
      printf("Floor\n");
      storage.push_back(floorMesh());
   } else if (file == "cube") {
      printf("Cube\n");
      storage.push_back(cubeMesh());
   } else {
   
      vector<glm::vec3> vertices;
      vector<glm::vec2> textures;
      vector<glm::vec2> texturesTemp;
      vector< vector<GLushort> > elements;
      
      bool objectStarted = false;
      vector<GLushort> triangles;
      bufferStore store;
      
      store.name = string(filename);
      
      ifstream in(filename, ios::in);
      ifstream matlib;
      if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
      
      string line;
      while (getline(in, line)) {
         if (line.substr(0,2) == "o ") {
            if (objectStarted) {
               elements.push_back(triangles);
               triangles.clear();
               /*for (int i = 0; i < elements.size(); i+=3) {
                  GLushort ia = elements[i];
                  GLushort ib = elements[i+1];
                  GLushort ic = elements[i+2];
                  glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                                                               glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
                  normals[ia] += normal;
                  normals[ib] += normal;
                  normals[ic] += normal;
               }
               
               newObject->BBBindBuffers(vertices, normals, elements, textures);
               newObjects->push_back(*newObject);
               normals.clear();
               elements.clear();
               textures.clear();
               newObject = new BBMatObject;*/
            }
            objectStarted = true;
         }
         else if (line.substr(0,2) == "v ") {
            istringstream s(line.substr(2));
            glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
            vertices.push_back(v);
         }  else if (line.substr(0,2) == "f ") {
            /*if (useMtl == 1) {
               string s = line.substr(2);
               string n;
               int txtCrd = 0;
               GLushort a;
               
               for (int i = 0; i <= s.length(); i++) {
                  if (s[i] != 47 && s[i] != 32 && i != s.length()) {
                     n += s[i];
                  } else {
                     istringstream str(n);
                     if (txtCrd == 0) {
                        str >> a;
                        a--;
                        //printf("Vertex %d has texture coordinates ", a);
                        elements.push_back(a);
                     } else {
                        int t;
                        str >> t;
                        //printf("(%0.3f, %0.3f)\n", texturesTemp.at(t-1).x, texturesTemp.at(t-1).y);
                        if (textures.at(a).x != texturesTemp.at(t-1).x && textures.at(a).x != 0.0) {
                           //printf("Replacing existing texture at %d\n", a+1);
                        }
                        textures.at(a).x = texturesTemp.at(t-1).x;
                        textures.at(a).y = texturesTemp.at(t-1).y;
                     }
                     n.clear();
                     txtCrd = (txtCrd + 1) % 2;
                  }
               }
            } else {*/
               istringstream s(line.substr(2));
               GLushort a,b,c;
               s >> a; s >> b; s >> c;
               a--; b--; c--;
               triangles.push_back(a); triangles.push_back(b); triangles.push_back(c);
            //}
         } else if (line.substr(0,3) == "vt ") {
            istringstream s(line.substr(3));
            glm::vec2 v; s >> v.x; s >> v.y;
            texturesTemp.push_back(v);
         } else if (line.substr(0,7) == "usemtl ") {
            if (line.substr(7).length() < 2) {
               //textures.resize(vertices.size(), vec2(0.0, 0.0));
            }
            
         } else if (line.substr(0,6) == "mtllib") {
            string mtl = "models/";
            mtl += line.substr(7);
            matlib.open(mtl.c_str(), ifstream::in);
         }
         else if (line[0] == '#') { /* ignoring this line */ }
         else { /* ignoring this line */ }
      }
      
      /*normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
      for (int i = 0; i < (int)elements.size(); i+=3) {
         //printf("Face %d: (%d, %d, %d)\n", i / 3, elements[i], elements[i+1], elements[i+2]);
         GLushort ia = elements[i];
         GLushort ib = elements[i+1];
         GLushort ic = elements[i+2];
         glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                                                      glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
         normals[ia] += normal;
         normals[ib] += normal;
         normals[ic] += normal;
      }
      newObject->BBBindBuffers(vertices, normals, elements, textures);
      //printf("%d\n", newObject->IndexBufferLength());
      newObjects->push_back(*newObject);*/
      elements.push_back(triangles);
      fillBuffer(&store, vertices, elements);
      
      storage.push_back(store);
   }
   
#ifdef DEBUG_VBO
   printf("VBO Transfered to Object Memory: %d\n", (int)storage[0].vertexBuffer);
#endif
}

int ModelManager::fillBuffer(bufferStore *store, vector<vec3> v, vector< vector<GLushort> > f)
{
   vector<vec3> normals;
   bound meshBound;
   float verts[v.size()*3];
   printf("Loading Verticies:\n");
   for (int i = 0; i < (int)v.size(); i++) {
      verts[i * 3] = v.at(i).x;
      verts[i * 3 + 1] = v.at(i).y;
      verts[i * 3 + 2] = v.at(i).z;
      
      meshBound.center.x += verts[i*3];
      meshBound.center.y += verts[i*3+1];
      meshBound.center.z += verts[i*3+2];
      
      /*if (verts[i*3] > boundingBoxMax.x) {
         boundingBoxMax.x = verts[i*3];
      } else if (verts[i*3] < boundingBoxMin.x) {
			boundingBoxMin.x = verts[i*3];
      }
      if (verts[i*3+1] > boundingBoxMax.y) {
         boundingBoxMax.y = verts[i*3+1];
      } else if (verts[i*3+1] < boundingBoxMin.y) {
			boundingBoxMin.y = verts[i*3+1];
      }
      if (verts[i*3+2] > boundingBoxMax.z) {
         boundingBoxMax.z = verts[i*3+2];
      } else if (verts[i*3+2] < boundingBoxMin.z) {
			boundingBoxMin.z = verts[i*3+2];
      }*/
      
       printf("   Vertex %d: (%0.3f, %0.3f, %0.3f)\n", i, verts[i], verts[i+1], verts[i+2]);
   }
   
   glGenBuffers(1, &store->vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, store->vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size()*3, verts, GL_STATIC_DRAW);
   
   normals.resize(v.size(), glm::vec3(0.0, 0.0, 0.0));
   float norms[normals.size()*3];
   GLushort *faces;
   store->numMeshes = f.size();
   store->indexBuffer = new GLuint[store->numMeshes];
   store->indexBufferLength = new int[store->numMeshes];
   for (int i = 0; i < (int)f.size(); i++) {
      faces = new GLushort[f[i].size()];
      store->indexBufferLength[i] = f[i].size();
      printf("   Mesh %d: %d faces\n", i, store->indexBufferLength[i]/3);
      for (int j = 0; j < (int)f[i].size(); j+=3) {
         GLushort ia = f[i][j];
         GLushort ib = f[i][j+1];
         GLushort ic = f[i][j+2];
         glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(v[ib]) - glm::vec3(v[ia]),
                                                         glm::vec3(v[ic]) - glm::vec3(v[ia])));
         normals[ia] += normal;
         normals[ib] += normal;
         normals[ic] += normal;
         
         faces[j] = f[i][j];
         faces[j+1] = f[i][j+1];
         faces[j+2] = f[i][j+2];
         printf("   Face %d: (%d, %d, %d)\n", j/3, faces[j], faces[j+1], faces[j+2]);
      }
       
      printf("Index Buffer %d Before: %d\n", i, (int)store->indexBuffer[i]);
      glGenBuffers(1, &store->indexBuffer[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, store->indexBuffer[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * f[i].size(), faces, GL_STATIC_DRAW);
      printf("Index Buffer %d After: %d\n", i, (int)store->indexBuffer[i]);
      
      delete[] faces;
   }

   for (int i = 0; i < (int)normals.size(); i++) {
      normals[i] = normalize(normals[i]);
      norms[i*3] = normals[i].x;
      norms[i*3+1] = normals[i].y;
      norms[i*3+2] = normals[i].z;
   }
   
   glGenBuffers(1, &store->normalBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, store->normalBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size()*3, norms, GL_STATIC_DRAW);
   
   store->textureBuffer = 0;
   
   /*
   printf("Mesh Info:\n");
   printf("   Number of Meshes: %d\n", store->numMeshes);
   for (int i = 0; i < store->numMeshes; i++) {
      printf("      Mesh %d has %d faces\n", i, store->indexBufferLength[i]);
   }
   */
   
   return 0;
}

bufferStore ModelManager::cubeMesh()
{
	bufferStore store;
	
   float verticies[72] = {
		-1.0, -1.0, -1.0,
      -1.0, -1.0, -1.0,
      -1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0, //3
      -1.0, 1.0, -1.0,
      -1.0, 1.0, -1.0,
		1.0, 1.0, -1.0, //6
      1.0, 1.0, -1.0,
      1.0, 1.0, -1.0,
		1.0, -1.0, -1.0, //9
      1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0, //12
      -1.0, -1.0, 1.0,
      -1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0, //15
      -1.0, 1.0, 1.0,
      -1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, //18
      1.0, 1.0, 1.0,
      1.0, 1.0, 1.0,
		1.0, -1.0, 1.0, //21
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
      1.0, 0.0, 0.0,
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
