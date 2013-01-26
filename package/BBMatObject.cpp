//
// BBMatObject.cpp
// Progam 01
//
// Created by Bryan Bell on 1/13/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines modeled game object.

#include "BBMatObject.h"
#include "GLSL_helper.h"
#include "math.h"
#include <GL/glfw.h>
#include <iostream>
#include <fstream>
#include <sstream>

BBMatObject::BBMatObject()
{
	diffColor = vec3(0.0, 0.0, 0.0);
	specColor = vec3(0.0, 0.0, 0.0);
	shaderFlags = vec4(0.0, 0.0, 0.0, 0.0);
	center = vec4(0.0, 0.0, 0.0, 0.0);
	boundingBoxMin = vec3(100.0, 100.0, 100.0);
	boundingBoxMax = vec3(-100.0, -100.0, -100.0);
	diffuse = 0.0;
	specularity = 0.0;
}

void BBMatObject::CopyMatObject(BBMatObject object)
{
	vertexBuffer = object.VertexBuffer();
	normalBuffer = object.NormalBuffer();
	indexBuffer = object.IndexBuffer();
	textureBuffer = object.TextureBuffer();
	normalMapBuffer = object.NormalMapBuffer();
	indexBufferLength = object.IndexBufferLength();
	
	textureLocation = object.TextureLocation();
	
	center = object.Center();
	boundingBoxMax = object.BoundingBoxMax();
	boundingBoxMin = object.BoundingBoxMin();
	
	diffColor = object.DiffColor();
	diffuse = object.Diffuse();
	specColor = object.SpecColor();
	specularity = object.Specularity();
	
	shaderFlags = object.ShaderFlags();
	
	scale = object.Scale();
	position = object.Position();
	rotation = object.Rotation();	
}


int BBMatObject::BBLoadMaterial(const char* fileName)
{
	ifstream in(fileName, ios::in);
   if (!in) { cerr << "Cannot open " << fileName << endl; exit(1); }
   
   string line;
   while (getline(in, line)) {
   		if(line.substr(0,3) == "Kd ") {
			istringstream s(line.substr(3));
         s >> diffColor.x; s >> diffColor.y; s >> diffColor.z; 
		} else if(line.substr(0,3) == "Ks ") {
			istringstream s(line.substr(3));
         s >> specColor.x; s >> specColor.y; s >> specColor.z; 
		} else if(line.substr(0,7) == "map_Kd ") {
   			glGenTextures(1, &textureLocation);
   			glBindTexture(GL_TEXTURE_2D, textureLocation);
   			printf("textureLocation: %d\n", (int)textureLocation);
   			
   			printf("%s\n", line.substr(7).c_str());
   			
   			glfwLoadTexture2D(line.substr(7).c_str(), GLFW_BUILD_MIPMAPS_BIT);
      		
      		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      		
      		shaderFlags.x = 1.0;
      		return 1;
		}
   }
   
   return 0;
}

void BBMatObject::BBDrawObject(GLint hPos, GLint hNor, GLint hTex, GLint hDif, GLint hSpc, GLint hShd, GLint hModel, GLint hNormal, mat4 modelMatrix)
{	
	if (shaderFlags.x == 1.0) {
		glEnable(GL_TEXTURE_2D);
	   glActiveTexture(textureLocation);
	   glBindTexture(GL_TEXTURE_2D, textureLocation);
	   //printf("textureLocation: %d\n", (int)textureLocation);
	   
	   safe_glEnableVertexAttribArray(hTex);
  		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  		safe_glVertexAttribPointer(hTex, 2, GL_FLOAT, GL_FALSE, 0, 0);
   
	}
   
   glUniform4f(hDif, diffColor.x, diffColor.y, diffColor.z, 1.0);
   glUniform4f(hSpc, specColor.x, specColor.y, specColor.z, 1.0);
   glUniform4f(hShd, shaderFlags.x, shaderFlags.y, shaderFlags.z, shaderFlags.w);
    
   safe_glEnableVertexAttribArray(hPos);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   safe_glVertexAttribPointer(hPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   safe_glEnableVertexAttribArray(hNor);
   glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
   safe_glVertexAttribPointer(hNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   mat4 normals = rotate(mat4(1.0), rotation.x, vec3(1.0, 0.0, 0.0));
   normals = rotate(normals, rotation.y, vec3(0.0, 1.0, 0.0));
   normals = rotate(normals, rotation.z, vec3(0.0, 0.0, 1.0));
   
   //printf("rot.y: %0.3f\n", rotation.y);
   
   //modelMatrix = scale(modelMatrix, scale);
   modelMatrix = translate(modelMatrix, position);
   modelMatrix = rotate(modelMatrix, rotation.x, vec3(1.0, 0.0, 0.0));
   modelMatrix = rotate(modelMatrix, rotation.y, vec3(0.0, 1.0, 0.0));
   modelMatrix = rotate(modelMatrix, rotation.z, vec3(0.0, 0.0, 1.0));
   
   // bind ibo
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
       
   safe_glUniformMatrix4fv(hModel, glm::value_ptr(modelMatrix));
   safe_glUniformMatrix4fv(hNormal, glm::value_ptr(normals));
   //printf("Here: %d\n", indexBufferLength);
   glDrawElements(GL_TRIANGLES, indexBufferLength, GL_UNSIGNED_SHORT, 0);
   
   safe_glDisableVertexAttribArray(hPos);
   safe_glDisableVertexAttribArray(hNor);
   if (shaderFlags.x == 1.0)
   		safe_glDisableVertexAttribArray(hTex);
}

void BBMatObject::BBBindBuffers(vector<glm::vec4> vertices, vector<glm::vec3> normals, vector<GLushort> elements, vector<glm::vec2> textures) {
    
   //printf("Faces: %ld\n", elements.size());
   float verts[vertices.size() * 3];
   float nrm[vertices.size() * 2];
   for (int i = 0; i < vertices.size(); i++) {
      verts[i * 3] = vertices.at(i).x;
      verts[i * 3 + 1] = vertices.at(i).y;
      verts[i * 3 + 2] = vertices.at(i).z;
      
      center.x += verts[i*3];
      center.y += verts[i*3+1];
      center.z += verts[i*3+2];
      
      nrm[i * 2] = -1.0;
      nrm[i * 2 + 1] = -1.0;
      
      if (verts[i*3] > boundingBoxMax.x) {
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
      	}
       printf("    Vertex %d: (%f, %f, %f)\n", i, verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
   }
  
  
   center /= vertices.size() * 3;
   vec3 calcCenter = vec3(center.x, center.y, center.z);
   
   for (int i = 0; i < vertices.size(); i++) {
   		vec3 vertex = vec3(vertices[i].x, vertices[i].y, vertices[i].z);
      if (abs(length(vertex - calcCenter)) > center.w)
      		center.w = abs(length(vertex - calcCenter));
   }
   
   /***********************************
   / Bounding Box and Center Info    
   /***********************************
   printf("Object Info:\n	Center: (%0.3f, %0.3f, %0.3f) Radius: %0.3f\n", 
    center.x, center.y, center.z, center.w);
   printf("	Bounding Box: (%0.3f, %0.3f) (%0.3f, %0.3f) (%0.3f, %0.3f)\n",
    boundingBoxMin.x, boundingBoxMax.x, boundingBoxMin.y, boundingBoxMax.y, boundingBoxMin.z, boundingBoxMax.z);
   */
   
   float norms[normals.size() * 3];
   for (int i = 0; i < normals.size(); i++) {
      norms[i * 3] = normals.at(i).x;
      norms[i * 3 + 1] = normals.at(i).y;
      norms[i * 3 + 2] = normals.at(i).z;
   }
   
   GLushort faces[elements.size()];
   float xText[4] = {1.0, 1.0, 0.0, 0.0};
   float yText[4] = {1.0, 0.0, 1.0, 0.0};
   int j = 0;
   for (int i = 0; i < elements.size(); i++) {
      faces[i] = elements.at(i);
      if (nrm[faces[i] * 2] == -1) {
         nrm[faces[i] * 2] = xText[j];
         nrm[faces[i] * 2 + 1] = yText[j++];
      } else {
         j++;
      }
      j %= 4;
   }
   
   for (int i = 0; i < elements.size(); i += 3) {
      //if (!textures.empty())
          printf("    Face %d: (%hu, %hu, %hu)\n", i / 3, faces[i], faces[i + 1], faces[i + 2]);
   }
    
   //printf("Binding: %s\n", model->name);
   
   //*******************************************
   //Make a BBMatObjectClass function
   // Getters and Setters make acquiring the Buffers' locations impossible
   //*******************************************
   indexBufferLength = (int)elements.size();
   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
   
   glGenBuffers(1, &indexBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
   
   glGenBuffers(1, &normalBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(norms), norms, GL_STATIC_DRAW);
   //printf("Here: %d\n", (int)vertices.size());
   /*glGenBuffers(1, &normalMapBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, normalMapBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(nrm), nrm, GL_STATIC_DRAW);*/
   if (!textures.empty()) {
      //printf("Textures for %s\n", model->name);
      float texts[textures.size() * 2];
      for (int i = 0; i < textures.size(); i++) {
         texts[i * 2] = textures.at(i).x;
         texts[i * 2 + 1] = textures.at(i).y;
         //printf("Text Point: (%0.3f, %0.3f)\n", texts[i*2], texts[i*2+1]);
      }
        
      glGenBuffers(1, &textureBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(texts), texts, GL_STATIC_DRAW);
   }
   //printf("%d\n", indexBufferLength);
   //printf("Finished: %s\n", model->name);
}