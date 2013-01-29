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

#include "KPPMeshManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

KPPMeshManager::KPPMeshManager(){
	//meshes = new vector<vector<meshes>>;
	//bounds = new vector<bounds>;
}

vector<Mesh> KPPMeshManager::getObj(const char *filename, vector<Mesh> **models, bound **boundingBox)
{
#if 0 
/*	vector<glm::vec3> vertices;
   vector<glm::vec3> normals;
   vector<glm::vec2> textures;
   vector<glm::vec2> texturesTemp;
   vector<vector<GLushort>> elements;
    
   bool objectStarted = false, useMtl = false;
   int numMeshes;
   mesh newMesh;
   ifstream in(filename, ios::in);
   if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
    
   string line;
   while (getline(in, line)) {
      if (line.substr(0,2) == "o ") {
      		newMesh.name = line.substr(2);
      		
         /*if (objectStarted == true) {
            normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
            
            for (int i = 0; i < elements.size(); i+=3) {
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
            newObject = new BBMatObject;
         }*/    
         //istringstream s(line.substr(2));
         //objectStarted = 1;
      }
      else if (line.substr(0,2) == "v ") {
         istringstream s(line.substr(2));
         glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
         vertices.push_back(v);
      } else if (line.substr(0,2) == "f ") {
         if (useMtl == true) {
            string s = line.substr(2);
            string n;
            int txtCrd = 0;
            GLushort a;
            
            for (int i = 0; i <= s.length(); i++) {
               if (s[i] != 47 && s[i] != 32 && i != s.length()) {
                  n += s[i];
               } else {
                  istringstream str(n);
                  if (txtCrd == false) {
                     str >> a;
                     elements.push_back(--a);
                     txtCrd = true;
                  } else {
                     int t;
                     str >> t;
                     
                     textures.at(a).x = texturesTemp.at(t-1).x;
                     textures.at(a).y = texturesTemp.at(t-1).y;
                     
                     txtCrd = false;
                  }
                  n.clear();
               }
            }
         } else {
            istringstream s(line.substr(2));
            GLushort a,b,c;
            s >> a; s >> b; s >> c;
            a--; b--; c--;
            elements.push_back(a); elements.push_back(b); elements.push_back(c);
         }
      } else if (line.substr(0,3) == "vt ") {
         istringstream s(line.substr(3));
         glm::vec2 v; s >> v.x; s >> v.y;
         texturesTemp.push_back(v);
      } else if (line.substr(0,7) == "usemtl ") {
         if (useMtl == true)
            textures.resize(vertices.size(), vec2(0.0, 0.0));
      } else if (line.substr(0,7) == "mtllib ") {
         string mtl = "models/";
         mtl += line.substr(7);
         if (newObject->BBLoadMaterial(mtl.c_str()) == 1) {
            useMtl = 1;
         } else {
        	   useMtl = 0;	
         }
//      } else if (line[0] == '#') { /* ignoring this line */ }
//      else { /* ignoring this line */ }



/*   }
    
   normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
   for (int i = 0; i < elements.size(); i+=3) {
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
   newObjects->push_back(*newObject);
    
   return 0;*/
#endif
   	
	if (modelList.empty()) {   	
   		loadObj("Hello!");
   		printf("Start-up mesh being loaded!\n");
   	}
   	
   	//models = 
   	
   	//bound = &bounds[0];
   	
   return modelList[0];   	
}

void KPPMeshManager::loadObj(const char *filename)
{
	vector<Mesh> newMeshes;
	newMeshes.push_back(cubeMesh());

	modelList.push_back(newMeshes);
	
	bound newBound;
	newBound.boundingBoxMax = vec3(1.0, 1.0, 1.0);
	newBound.boundingBoxMin = vec3(-1.0, -1.0, -1.0);
	newBound.center = vec3(0.0, 0.0, 0.0);
	newBound.radius = sqrt(3);
	
	bounds.push_back(newBound);
}

Mesh KPPMeshManager::cubeMesh()
{
	Mesh *newMesh = new Mesh();
	
   float vertecies[24] = {
		-1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,
		1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, -1.0, 1.0
   	};
   	
   	float normals[24] = {
		-1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,
		1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, -1.0, 1.0
   	};
   	
   float faces[36] = {
		0, 3, 1,
		2, 1, 3,
		0, 5, 4,
		0, 1, 5,
		2, 3, 6,
		6, 3, 7,
		5, 6, 7, 
		5, 7, 4,
		1, 2, 6,
		1, 6, 5,
		4, 7, 0,
		7, 0, 3   	
   	};
  
   vec3 diff = vec3(1.0, 0.0, 0.0);
   float spec = 0.3;
   
   GLuint vbo, ibo, nbo;

   int ibl = 12;
   
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);
   
   glGenBuffers(1, &ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
   
   glGenBuffers(1, &nbo);
   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
   
   newMesh->init(vbo, nbo, ibo, ibl, diff, spec);
   
   return *newMesh;
}