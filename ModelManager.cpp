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
#include "LoadTexture.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

vec3 l_direction;

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
   if (objOne.center.x - objOne.dimension.x > objTwo.center.x + objTwo.dimension.x ||
       objOne.center.x + objOne.dimension.x < objTwo.center.x - objTwo.dimension.x ||
       objOne.center.y - objOne.dimension.y > objTwo.center.y + objTwo.dimension.y ||
       objOne.center.y + objOne.dimension.y < objTwo.center.y - objTwo.dimension.y ||
       objOne.center.z - objOne.dimension.z > objTwo.center.z + objTwo.dimension.z ||
       objOne.center.z + objOne.dimension.z < objTwo.center.z - objTwo.dimension.z) {
      return false;
   }
   
   return true;
}


ModelManager::ModelManager()
{
   l_direction = vec3(0.707, -0.707, 0);
}

ModelManager::~ModelManager()
{
   for (int i = 0; i < (int)storage.size(); i++) {
      delete[] storage[i].indexBuffer;
      delete[] storage[i].indexBufferLength;
      delete[] storage[i].material;
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
   
   meshes->name = storage[index].name;
   	
   meshes->vertexBuffer = storage[index].vertexBuffer;
   meshes->normalBuffer = storage[index].normalBuffer;
   meshes->textureBuffer = storage[index].textureBuffer;
   if (storage[index].shadowVBO != NULL)
      meshes->shadowVBO = storage[index].shadowVBO;
   else
      meshes->shadowVBO = NULL;
   
   meshes->indexBuffer = new GLuint[storage[index].numMeshes];
   meshes->shadowIBO = new GLuint[storage[index].numMeshes];
   meshes->shadowBufferLength = new int[storage[index].numMeshes];
   meshes->indexBufferLength = new int[storage[index].numMeshes];
   meshes->material = new PhongMaterial[storage[index].numMeshes];
   
   meshes->numMeshes = storage[index].numMeshes;
   
   for (int i = 0; i < meshes->numMeshes; i++) {
      meshes->indexBuffer[i] = storage[index].indexBuffer[i];
      meshes->indexBufferLength[i] = storage[index].indexBufferLength[i];
      if (storage[index].shadowVBO != NULL) {
         meshes->shadowIBO[i] = storage[index].shadowIBO[i];
         meshes->shadowBufferLength[i] = storage[index].shadowBufferLength[i];
      } else {
         meshes->shadowIBO = NULL;
         meshes->shadowBufferLength = 0;
      }
      
      meshes->material[i] = storage[index].material[i];
      // meshes->diffuseColor[i] = storage[index].diffuseColor[i];
      // meshes->specularity[i] = storage[index].specularity[i];
   }
   
   /*
   printf("Mesh Info:\n");
   printf("   Number of Meshes: %d\n", meshes->numMeshes);
   for (int i = 0; i < meshes->numMeshes; i++) {
      printf("      Mesh %d has %d index\n", i, (int)meshes->indexBuffer[i]);
   }
   */
   
   //boundingInfo->bottomLeft = boundStorage[index].bottomLeft;
   boundingInfo->dimension = boundStorage[index].dimension;
   boundingInfo->center = boundStorage[index].center;
   boundingInfo->radius = boundStorage[index].radius;

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
      
      bound newBound;
      //newBound.bottomLeft = vec3(-1.0, -1.0, -1.0);
      newBound.dimension = vec3(1.0, 1.0, 1.0);
      newBound.center = vec3(0.0, 0.0, 0.0);
      newBound.radius = 1.0;
      boundStorage.push_back(newBound);
   } else if (file == "floor") {
      printf("Floor\n");
      storage.push_back(floorMesh());
      
      bound newBound;
      //newBound.bottomLeft = vec3(-1.0, 0.0, -1.0);
      newBound.dimension = vec3(1.0, 0.005, 1.0);
      newBound.center = vec3(0.0, 0.0, 0.0);
      newBound.radius = 1.0;
      boundStorage.push_back(newBound);
   } else if (file == "plane") {
      printf("Plane\n");
      storage.push_back(planeMesh());
      
      bound newBound;
      //newBound.bottomLeft = vec3(-0.5, -0.5, 1.0);
      newBound.dimension = vec3(0.5, 0.5, 0.005);
      newBound.center = vec3(0.0, 0.0, 1.0);
      newBound.radius = 1.0;
      boundStorage.push_back(newBound);
   } else if (file == "cube") {
      printf("Cube\n");
      storage.push_back(cubeMesh());
      
      bound newBound;
      //newBound.bottomLeft = vec3(-1.0, -1.0, -1.0);
      newBound.dimension = vec3(1.0, 1.0, 1.0);
      newBound.center = vec3(0.0, 0.0, 0.0);
      newBound.radius = 1.0;
      boundStorage.push_back(newBound);
   } else {
   
      vector<glm::vec3> vertices;
      vector<glm::vec2> textures;
      vector<glm::vec2> texturesTemp;
      vector< vector<GLushort> > elements;
      vector<PhongMaterial> materials;
      
      bool objectStarted = false;
      vector<GLushort> triangles;
      bufferStore store;
      
      store.name = string(filename);
      
      ifstream in(filename, ios::in);
      ifstream *matlib = new ifstream();
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
            textures.resize(vertices.size());
            if (materials[materials.size()-1].textureLocation != 0) {
               string s = line.substr(2);
               string n;
               int txtCrd = 0;
               GLushort a;
               
               for (unsigned int i = 0; i <= s.length(); i++) {
                  if (s[i] != 47 && s[i] != 32 && i != s.length()) {
                     n += s[i];
                  } else {
                     istringstream str(n);
                     if (txtCrd == 0) {
                        str >> a;
                        a--;
                        //printf("Vertex %d has texture coordinates ", a);
                        triangles.push_back(a);
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
            } else {
               istringstream s(line.substr(2));
               GLushort a,b,c;
               s >> a; s >> b; s >> c;
               a--; b--; c--;
               triangles.push_back(a); triangles.push_back(b); triangles.push_back(c);
            }
         } else if (line.substr(0,3) == "vt ") {
            istringstream s(line.substr(3));
            glm::vec2 v; s >> v.x; s >> v.y;
            texturesTemp.push_back(v);
         } else if (line.substr(0,6) == "usemtl") {
            PhongMaterial newMat;
            
            matlib->seekg (0, ios::beg);
            getMaterial(matlib, line.substr(7), &newMat);
            
            printf("\nMaterial for %s:\n", store.name.c_str());
            printf("   Diffuse: (%0.3f, %0.3f, %0.3f)\n", newMat.dColor.x,
                   newMat.dColor.y, newMat.dColor.z);
            printf("   Specular: (%0.3f, %0.3f, %0.3f)\n", newMat.sColor.x,
                   newMat.sColor.y, newMat.sColor.z);
            printf("   Ambient: (%0.3f, %0.3f, %0.3f)\n\n", newMat.aColor.x,
                   newMat.aColor.y, newMat.aColor.z);
            /*if (getMaterial(matlib, line.substr(7), &newMat) != 1) {
               newMat.aColor = vec3(1.0, 0.0, 0.0);
               newMat.sColor = vec3(0.0, 1.0, 0.0);
               newMat.dColor = vec3(0.0, 0.0, 1.0);
               newMat.shine = 0.5;
            }*/
            
            materials.push_back(newMat);
         } else if (line.substr(0,6) == "mtllib") {
            string mtl = "models/";
            mtl += line.substr(7);
            matlib->open(mtl.c_str(), ifstream::in);
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
      fillBuffer(&store, vertices, textures, elements, materials);
      
      storage.push_back(store);
   }
   
#ifdef DEBUG_VBO
   printf("VBO Transfered to Object Memory: %d\n", (int)storage[0].vertexBuffer);
#endif
}

int ModelManager::getMaterial(ifstream *matlib, string name, PhongMaterial *mat)
{
   string line;
   bool matFound = false;
   matlib->seekg (0, ios::beg);
   
   while (getline(*matlib, line)) {
      if (line.substr(0,7) == "newmtl " && !matFound) {
         string found = line.substr(7);
         printf("Found: %s Attempt: %s\n", found.c_str(), name.c_str());
         if (found == name) {
            matFound = true;
            while (getline(*matlib, line)) {
               if (line.substr(0,3) == "Ka ") {
                  istringstream s(line.substr(3));
                  s >> mat->aColor.x; s >> mat->aColor.y; s >> mat->aColor.z;
               } else if (line.substr(0,3) == "Kd ") {
                  istringstream s(line.substr(3));
                  s >> mat->dColor.x; s >> mat->dColor.y; s >> mat->dColor.z;
               } else if (line.substr(0,3) == "Ks ") {
                  istringstream s(line.substr(3));
                  s >> mat->sColor.x; s >> mat->sColor.y; s >> mat->sColor.z;
               } else if (line.substr(0,6) == "illum ") {
                  getline(*matlib, line);
                  if(line.substr(0,7) == "map_Kd ") {
                     glGenTextures(1, &mat->textureLocation);
                     glBindTexture(GL_TEXTURE_2D, mat->textureLocation);
                     printf("textureLocation: %d\n", (int)mat->textureLocation);
                     
                     printf("%s\n", line.substr(7).c_str());
                     
                     LoadTexture(line.substr(7).c_str(), mat->textureLocation);
                     
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                     
                     printf("Hello\n");
                     mat->dColor.x = -1.0; mat->dColor.y = -1.0; mat->dColor.z = -1.0;
                  } else {
                     mat->textureLocation = 0;
                  }
                  return 1;
               }
            }
         }
      }
   }
   
   mat->textureLocation = 0;
   
   return 0;
}

#define SHADOW_INFINITY 1.0f
int ModelManager::fillBuffer(bufferStore *store, vector<vec3> v, vector<vec2> t, vector< vector<GLushort> > f, vector<PhongMaterial> materials)
{
   vector<vec3> normals;
   vector<vec3> shadowVerts;
   vector< vector<GLushort> > shadowFaces;
   vector< vector<int> > shadowInfo;
   bound meshBound;
   float verts[v.size()*3];
   //float texts[t.size()*2];
   
   vec3 bottomLeft = vec3(-100.0, -100.0, -100.0);
   vec3 topRight = vec3(-100.0, -100.0, -100.0);
   
   //printf("Loading Verticies:\n");
   for (int i = 0; i < (int)v.size(); i++) {
      verts[i * 3] = v.at(i).x;
      verts[i * 3 + 1] = v.at(i).y;
      verts[i * 3 + 2] = v.at(i).z;
      
      /*texts[i * 2] = t.at(i).x;
      texts[i * 2 + 1] = t.at(i).y;
      printf("Texture Coordinate %d: (%0.3f, %0.3f)\n", i, texts[i*2], texts[i*2+1]);*/
      
      
      meshBound.center.x += verts[i*3];
      meshBound.center.y += verts[i*3+1];
      meshBound.center.z += verts[i*3+2];
      
      if (verts[i*3] < bottomLeft.x) {
         bottomLeft.x = verts[i*3];
      } else if (verts[i*3] > topRight.x) {
			topRight.x = verts[i*3];
      }
      if (verts[i*3+1] < bottomLeft.y) {
         bottomLeft.y = verts[i*3+1];
      } else if (verts[i*3+1] > topRight.y) {
			topRight.y = verts[i*3+1];
      }
      if (verts[i*3+2] < bottomLeft.z) {
         bottomLeft.z = verts[i*3+2];
      } else if (verts[i*3+2] > topRight.z) {
			topRight.z = verts[i*3+2];
      }
      
      //printf("   Vertex %d: (%0.3f, %0.3f, %0.3f)\n", i, verts[i], verts[i+1], verts[i+2]);
   }
   
   meshBound.dimension.x = std::max(topRight.x, bottomLeft.x);
   meshBound.dimension.y = std::max(topRight.y, bottomLeft.y);
   meshBound.dimension.z = std::max(topRight.z, bottomLeft.z);
   
   meshBound.center.x = meshBound.center.x / v.size();
   meshBound.center.y = meshBound.center.y / v.size();
   meshBound.center.z = meshBound.center.z / v.size();
   
   meshBound.radius = std::max(std::max(meshBound.dimension.x, meshBound.dimension.y),
                               meshBound.dimension.z); //length(meshBound.dimension);
   
   glGenBuffers(1, &store->vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, store->vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size()*3, verts, GL_STATIC_DRAW);
   
   /*for (int i = 0; i < store->numMeshes; i++) {
      if (store->material[i].textureLocation != 0) {
         glGenBuffers(1, &store->material[i].textureCoordinates);
         glBindBuffer(GL_ARRAY_BUFFER, store->material[i].textureCoordinates);
         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * t.size()*2, texts, GL_STATIC_DRAW);
      }
   }*/
   
   normals.resize(v.size(), glm::vec3(0.0, 0.0, 0.0));
   float norms[normals.size()*3];
   GLushort *faces;
   store->numMeshes = f.size();
   store->indexBuffer = new GLuint[store->numMeshes];
   store->indexBufferLength = new int[store->numMeshes];
   store->shadowBufferLength = new int[store->numMeshes];
   store->shadowIBO = new GLuint[store->numMeshes];
   for (int i = 0; i < (int)f.size(); i++) {
      faces = new GLushort[f[i].size()];
      vector<int> sInfo;
      vector<GLushort> sFaces;
      store->indexBufferLength[i] = f[i].size();
      //printf("   Mesh %d: %d faces\n", i, store->indexBufferLength[i]/3);
      for (int j = 0; j < (int)f[i].size(); j+=3) {
         GLushort ia = f[i][j];
         GLushort ib = f[i][j+1];
         GLushort ic = f[i][j+2];
         glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(v[ib]) - glm::vec3(v[ia]),
                                                         glm::vec3(v[ic]) - glm::vec3(v[ia])));
         normals[ia] += normal;
         normals[ib] += normal;
         normals[ic] += normal;
         
         if (dot(normal, l_direction) < 0) {
            sInfo.push_back(-1);
         } else {
            sInfo.push_back(1);
         }
         
         faces[j] = f[i][j];
         faces[j+1] = f[i][j+1];
         faces[j+2] = f[i][j+2];
         //printf("   Face %d: (%d, %d, %d)\n", j/3, faces[j], faces[j+1], faces[j+2]);
      }
      shadowInfo.push_back(sInfo);
      
      /* Shadow Info
      for (int j = 0; j < (int)f[i].size()/3; j++) {
         printf("shadowInfo[%d][%d]: %d\n", i, j, shadowInfo[i][j]);
      }
      printf("sInfo : f[i] == %d : %d\n", shadowInfo[i].size(), f[i].size());*/
      
      vector<vec2> vertInfo; // { v.index, shadowVerts.index }
      vec3 centroid = vec3(0, 0, 0);
      int centroidCount = 0, currentSize = 1, initSize = shadowVerts.size();
      shadowVerts.push_back(centroid);
      for (int j = 0; j < (int)f[i].size()/3; j++) {
         if (shadowInfo[i][j] == -1) {
            int v1 = -1, v2 = -1;
            bool found = false;
            for (int k = 0; k < (int)f[i].size()/3 && !found; k++) {
               if (shadowInfo[i][k] == 1) {
                  if (f[i][j*3] == f[i][k*3] || f[i][j*3] == f[i][k*3+1]
                      || f[i][j*3] == f[i][k*3+2]) {
                     v1 = f[i][j*3];
                     if (f[i][j*3+1] == f[i][k*3] || f[i][j*3+1] == f[i][k*3+1]
                         || f[i][j*3+1] == f[i][k*3+2]) {
                        v2 = f[i][j*3+1];
                        found = true;
                     } else if (f[i][j*3+2] == f[i][k*3] || f[i][j*3+2] == f[i][k*3+1]
                                || f[i][j*3+2] == f[i][k*3+2]) {
                        v2 = f[i][j*3+2];
                        found = true;
                     } else {
                        v1 = -1; v2 = -1;
                     }
                  }
               }
            }
            
            if (v1 != -1) {
               vertInfo.push_back(vec2(v1, shadowVerts.size()));
               shadowVerts.push_back(v[v1]);
               v1 = shadowVerts.size() - 1;
               currentSize++;
               
               vertInfo.push_back(vec2(v2, shadowVerts.size()));
               shadowVerts.push_back(v[v2]);
               v2 = shadowVerts.size() - 1;
               currentSize++;
               
               sFaces.push_back(v1);
               sFaces.push_back(initSize);
               sFaces.push_back(v2);
               
               centroid += shadowVerts[v1];
               centroid += shadowVerts[v2];
               
               centroidCount += 2;
            }

            /*if (dot(normalize(normals[f[i][j*3]]), l_direction) >= 0) {
               if (dot(normalize(normals[f[i][j*3+1]]), l_direction) >= 0) {
                  for (int l = 0; l < (int)vertInfo.size(); l++) {
                     if (vertInfo[l].x == f[i][j*3])
                        v1 = vertInfo[l].y;
                     else if (vertInfo[l].x == f[i][j*3+1])
                        v2 = vertInfo[l].y;
                  }
                  
                  if (v1 == -1) {
                     vertInfo.push_back(vec2(f[i][j*3], shadowVerts.size()));
                     v1 = shadowVerts.size();
                     shadowVerts.push_back(v[f[i][j*3]]);
                     currentSize++;
                  }
                  if (v2 == -1) {
                     vertInfo.push_back(vec2(f[i][j*3+1], shadowVerts.size()));
                     v2 = shadowVerts.size();
                     shadowVerts.push_back(v[f[i][j*3+1]]);
                     currentSize++;
                  }
                  
                  sFaces.push_back(v1);
                  sFaces.push_back(initSize);
                  sFaces.push_back(v2);
                  
                  centroid += shadowVerts[v1];
                  centroid += shadowVerts[v2];
                  
                  centroidCount +=2;
                  
               } else if (dot(normalize(normals[f[i][j*3+2]]), l_direction) >= 0) {
                  for (int l = 0; l < (int)vertInfo.size(); l++) {
                     if (vertInfo[l].x == f[i][j*3])
                        v1 = vertInfo[l].y;
                     else if (vertInfo[l].x == f[i][j*3+2])
                        v2 = vertInfo[l].y;
                  }
                  
                  if (v1 == -1) {
                     vertInfo.push_back(vec2(f[i][j*3], shadowVerts.size()));
                     v1 = shadowVerts.size();
                     shadowVerts.push_back(v[f[i][j*3]]);
                     currentSize++;
                  }
                  if (v2 == -1) {
                     vertInfo.push_back(vec2(f[i][j*3+2], shadowVerts.size()));
                     v2 = shadowVerts.size();
                     shadowVerts.push_back(v[f[i][j*3+2]]);
                     currentSize++;
                  }
                  
                  sFaces.push_back(v1);
                  sFaces.push_back(initSize);
                  sFaces.push_back(v2);
                  
                  centroid += shadowVerts[v1];
                  centroid += shadowVerts[v2];
                  
                  centroidCount +=2;
               }
            } else if (dot(normalize(normals[f[i][j*3+1]]), l_direction) >= 0 && dot(normalize(normals[f[i][j*3+2]]), l_direction) >= 0) {
               for (int l = 0; l < (int)vertInfo.size(); l++) {
                  if (vertInfo[l].x == f[i][j*3+1])
                     v1 = vertInfo[l].y;
                  else if (vertInfo[l].x == f[i][j*3+2])
                     v2 = vertInfo[l].y;
               }
               
               if (v1 == -1) {
                  vertInfo.push_back(vec2(f[i][j*3+1], shadowVerts.size()));
                  v1 = shadowVerts.size();
                  shadowVerts.push_back(v[f[i][j*3+2]]);
                  currentSize++;
               }
               if (v2 == -1) {
                  vertInfo.push_back(vec2(f[i][j*3+1], shadowVerts.size()));
                  v2 = shadowVerts.size();
                  shadowVerts.push_back(v[f[i][j*3+2]]);
                  currentSize++;
               }
               
               sFaces.push_back(v1);
               sFaces.push_back(initSize);
               sFaces.push_back(v2);
               
               centroid += shadowVerts[v1];
               centroid += shadowVerts[v2];
               
               centroidCount +=2;
            }*/
         }
      }
      
      centroid /= centroidCount;
      
      shadowVerts[initSize] = centroid;
      
      for (int j = 0; j < currentSize; j++) {
         vec3 extend = shadowVerts[initSize + j];
         extend += l_direction * SHADOW_INFINITY;
         shadowVerts.push_back(extend);
      }
      
      int faceSize = sFaces.size();
      for (int j = 0; j < faceSize; j++) {
         sFaces.push_back(sFaces[j] + currentSize);
      }
      
      for (int j = 0; j < faceSize; j += 3) {
         sFaces.push_back(sFaces[j]);
         sFaces.push_back(sFaces[j] + currentSize);
         sFaces.push_back(sFaces[j+2] + currentSize);
         
         sFaces.push_back(sFaces[j]);
         sFaces.push_back(sFaces[j+2] + currentSize);
         sFaces.push_back(sFaces[j+2]);
      }
      
      shadowFaces.push_back(sFaces);
      
      store->shadowBufferLength[i] = sFaces.size();
      
      
      GLushort sdwFaces[sFaces.size()];
      
      for (int j = 0; j < (int)sFaces.size(); j++) {
         sdwFaces[j] = (GLushort)sFaces[j];
      }
      
      /* Shadow Faces
      printf("Number of Verticies: %d\n", (int)shadowVerts.size());
      for (int j = 0; j < (int)sFaces.size(); j+=3) {
         printf("   Face %d: %d, %d, %d\n", j/3, sdwFaces[j], sdwFaces[j+1], sdwFaces[j+2]);
      }*/
      //Extend verticies/faces in direction of light
      //connect verticies into faces
      
      //create shadow vertex array and face array
      
      glGenBuffers(1, &store->shadowIBO[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, store->shadowIBO[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * sFaces.size(), sdwFaces, GL_STATIC_DRAW);
       
      //printf("Index Buffer %d Before: %d\n", i, (int)store->indexBuffer[i]);
      glGenBuffers(1, &store->indexBuffer[i]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, store->indexBuffer[i]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * f[i].size(), faces, GL_STATIC_DRAW);
      //printf("Index Buffer %d After: %d\n", i, (int)store->indexBuffer[i]);
      
      delete[] faces;
   }
   
   float sdwVerts[shadowVerts.size()*3];
   for (int i = 0; i < (int)shadowVerts.size(); i++) {
      sdwVerts[i*3] = shadowVerts[i].x;
      sdwVerts[i*3+1] = shadowVerts[i].y;
      sdwVerts[i*3+2] = shadowVerts[i].z;
   }
   
   /* Shadow Verts
   printf("Number of Verticies: %d\n", (int)shadowVerts.size());
   for (int j = 0; j < (int)shadowVerts.size(); j++) {
      printf("   Vertex %d: %0.3f, %0.3f, %0.3f\n", j, sdwVerts[j*3], sdwVerts[j*3+1], sdwVerts[j*3+2]);
   }*/
   
   glGenBuffers(1, &store->shadowVBO);
   glBindBuffer(GL_ARRAY_BUFFER, store->shadowVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shadowVerts.size()*3, sdwVerts, GL_STATIC_DRAW);

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
   
   store->material = new PhongMaterial[materials.size()];
   
   for (int i = 0; i < (int)materials.size(); i++) {
      store->material[i] = materials.at(i);
      
      printf("\nMaterial for %s:\n", store->name.c_str());
      printf("   Diffuse: (%0.3f, %0.3f, %0.3f)\n", store->material[i].dColor.x,
             store->material[i].dColor.y, store->material[i].dColor.z);
      printf("   Specular: (%0.3f, %0.3f, %0.3f)\n", store->material[i].sColor.x,
             store->material[i].sColor.y, store->material[i].sColor.z);
      printf("   Ambient: (%0.3f, %0.3f, %0.3f)\n\n", store->material[i].aColor.x,
             store->material[i].aColor.y, store->material[i].aColor.z);
   }
   
   boundStorage.push_back(meshBound);
   
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
   store.material = new PhongMaterial[6];
   
   store.numMeshes = 6;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.material[0].aColor = vec3(0.0, 0.6, 0.0);
   store.material[0].dColor = vec3(0.0, 0.6, 0.0);
   store.material[0].sColor = vec3(0.0, 0.6, 0.0);
   store.material[0].shine = 0.5;
   
   store.indexBuffer[1] = ibo2;
   store.indexBufferLength[1] = ibl;
   store.material[1].aColor = vec3(0.6, 0.6, 0.0);
   store.material[1].dColor = vec3(0.6, 0.6, 0.0);
   store.material[1].sColor = vec3(0.6, 0.6, 0.0);
   store.material[1].shine = 0.5;
   
   store.indexBuffer[2] = ibo3;
   store.indexBufferLength[2] = ibl;
   store.material[2].aColor = vec3(0.0, 0.6, 0.0);
   store.material[2].dColor = vec3(0.0, 0.6, 0.0);
   store.material[2].sColor = vec3(0.0, 0.6, 0.0);
   store.material[2].shine = 0.5;
   
   store.indexBuffer[3] = ibo4;
   store.indexBufferLength[3] = ibl;
   store.material[3].aColor = vec3(0.0, 0.6, 0.0);
   store.material[3].dColor = vec3(0.0, 0.6, 0.0);
   store.material[3].sColor = vec3(0.0, 0.6, 0.0);
   store.material[3].shine = 0.5;
   
   store.indexBuffer[4] = ibo5;
   store.indexBufferLength[4] = ibl;
   store.material[4].aColor = vec3(0.8, 0.4, 0.0);
   store.material[4].dColor = vec3(0.8, 0.4, 0.0);
   store.material[4].sColor = vec3(0.8, 0.4, 0.0);
   store.material[4].shine = 0.5;
   
   store.indexBuffer[5] = ibo6;
   store.indexBufferLength[5] = ibl;
   store.material[5].aColor = vec3(0.8, 0.4, 0.0);
   store.material[5].dColor = vec3(0.8, 0.4, 0.0);
   store.material[5].sColor = vec3(0.8, 0.4, 0.0);
   store.material[5].shine = 0.5;
   
   store.name = "cube";
   
   store.shadowBufferLength = NULL;
   store.shadowVBO = NULL;
   store.shadowIBO = NULL;
   
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
   store.material = new PhongMaterial[6];
   
   store.numMeshes = 5;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.material[0].aColor = vec3(0.8, 0.8, 0.8);
   store.material[0].dColor = vec3(0.8, 0.8, 0.8);
   store.material[0].sColor = vec3(0.8, 0.8, 0.8);
   store.material[0].shine = 0.5;
   
   store.indexBuffer[1] = ibo2;
   store.indexBufferLength[1] = ibl;
   store.material[1].aColor = vec3(0.8, 0.8, 0.8);
   store.material[1].dColor = vec3(0.8, 0.8, 0.8);
   store.material[1].sColor = vec3(0.8, 0.8, 0.8);
   store.material[1].shine = 0.5;
   
   store.indexBuffer[2] = ibo3;
   store.indexBufferLength[2] = ibl2;
   store.material[2].aColor = vec3(0.8, 0.8, 0.8);
   store.material[2].dColor = vec3(0.8, 0.8, 0.8);
   store.material[2].sColor = vec3(0.8, 0.8, 0.8);
   store.material[2].shine = 0.5;
   
   store.indexBuffer[3] = ibo4;
   store.indexBufferLength[3] = ibl2;
   store.material[3].aColor = vec3(0.8, 0.8, 0.8);
   store.material[3].dColor = vec3(0.8, 0.8, 0.8);
   store.material[3].sColor = vec3(0.8, 0.8, 0.8);
   store.material[3].shine = 0.5;
   
   store.indexBuffer[4] = ibo5;
   store.indexBufferLength[4] = ibl;
   store.material[4].aColor = vec3(0.8, 0.8, 0.8);
   store.material[4].dColor = vec3(0.8, 0.8, 0.8);
   store.material[4].sColor = vec3(0.8, 0.8, 0.8);
   store.material[4].shine = 0.5;
   
   store.shadowBufferLength = NULL;
   store.shadowVBO = NULL;
   store.shadowIBO = NULL;
   
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
   store.material = new PhongMaterial[6];
   
   store.numMeshes = 1;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.material[0].aColor = vec3(0.2, 0.1, 0.0);
   store.material[0].dColor = vec3(0.5, 0.3, 0.0);
   store.material[0].sColor = vec3(0.1, 0.1, 0.0);
   store.material[0].shine = 0.5;
   
   store.name = "floor";
   
   store.shadowBufferLength = NULL;
   store.shadowVBO = NULL;
   store.shadowIBO = NULL;
   
#ifdef DEBUG_VBO
   printf("VBO Transfered to Local Variable \"store\": %d\n", (int)store.indexBufferLength[0]);
#endif
   return store;
}

bufferStore ModelManager::planeMesh()
{
	bufferStore store;
	
   float verticies[12] = {
		-0.5, -0.5, 0.0,
      -0.5, 0.5, 0.0,
      0.5, -0.5, 0.0,
		0.5, 0.5, 0.0
   };
   
   float normals[12] = {
		0.0, 0.0, 1.0,
      0.0, 0.0, 1.0,
      0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
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
   store.material = new PhongMaterial[6];
   
   store.numMeshes = 1;
   
   store.indexBuffer[0] = ibo;
   store.indexBufferLength[0] = ibl;
   store.material[0].aColor = vec3(0.3, 0.0, 0.3);
   store.material[0].dColor = vec3(0.6, 0.0, 0.6);
   store.material[0].sColor = vec3(0.8, 0.0, 0.8);
   store.material[0].shine = 0.5;
   
   store.name = "plane";
   
   store.shadowBufferLength = NULL;
   store.shadowVBO = NULL;
   store.shadowIBO = NULL;
   
#ifdef DEBUG_VBO
   printf("VBO Transfered to Local Variable \"store\": %d\n", (int)store.indexBufferLength[0]);
#endif
   return store;
}
