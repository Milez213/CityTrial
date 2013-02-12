#ifndef HUD_SHADER_H
#define HUD_SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glfw.h>

#include "Shader.h"
#include "defines.h"
#include "LoadTexture.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using glm::vec3;
using glm::mat4;
using glm::value_ptr;
using namespace std;

class HUDShader : public Shader {

public:

    HUDShader() : Shader("shaders/hud_vert.glsl", "shaders/hud_frag.glsl") {
        if (m_installed) {
            // matrices
            h_uModelMatrix = safe_glGetUniformLocation(m_shaderProg, "uModelMatrix");
            h_uProjMatrix = safe_glGetUniformLocation(m_shaderProg, "uProjMatrix");
            h_uViewMatrix = safe_glGetUniformLocation(m_shaderProg, "uViewMatrix");

            // attributes
            h_aPosition = safe_glGetAttribLocation(m_shaderProg, "aPosition");

            // TODO - add texture coordinates handle in shader
            h_aTexture = safe_glGetAttribLocation(m_shaderProg, "aTexture");
           
           loadCoordinates("textures/hudMap.crd");
           
           GLfloat verticies[12] = {
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0,
              1.0, 0.0, 0.0,
              1.0, 1.0, 0.0
           };
           
           /*GLfloat verticies[12] = {
              -1.0, -1.0, 0.0,
              -1.0, 1.0, 0.0,
              1.0, -1.0, 0.0,
              1.0, 1.0, 0.0
           };*/
           
           GLfloat text[8] = {
              0.0, 1.0,
              0.0, 0.0,
              1.0, 1.0,
              1.0, 0.0
           };
           
           GLushort faces[6] = {
              0, 3, 1,
              0, 3, 2
           };
           
           glGenBuffers(1, &planeCoord);
           glBindBuffer(GL_ARRAY_BUFFER, planeCoord);
           glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
           
           glGenBuffers(1, &textCoord);
           glBindBuffer(GL_ARRAY_BUFFER, textCoord);
           glBufferData(GL_ARRAY_BUFFER, sizeof(text), text, GL_STATIC_DRAW);
           
           glGenBuffers(1, &elements);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
           glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
           
 
           texture = 0;
           LoadTextureAlpha("textures/hudAtlas.bmp", texture);
           // There's also LoadTextureAlpha which loads loads 32-bit RGBA BMP's
           
           
           /*
           // too slow
           glGenTextures(1, &texture);
           glBindTexture(GL_TEXTURE_2D, texture);
           
           glfwLoadTexture2D("textures/hudAtlas.tga", 0);
           
           already done in LoadTexture
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           */
        } else {
            fprintf(stderr, "HUDShader(): Couldn't install shaders\n");
            exit(1);
        }
    }
   
    void makeActive() {
       glEnable(GL_TEXTURE_2D);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, texture);
       setTexture(0);
       
       safe_glEnableVertexAttribArray(h_aPosition);
       glBindBuffer(GL_ARRAY_BUFFER, planeCoord);
       safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
   
    void draw(string name) {
       int index = -1;
       for (int i = 0; i < numCoords && index == -1; i++) {
          if (name == names[i])
             index = i;
       }
       
       if (index == -1) {
          printf("No Coordinates Mapped\n");
          index = 0;
       }
       
       glEnable(GL_TEXTURE_2D);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, texture);
       //setTexture(texture);
       
       safe_glEnableVertexAttribArray(h_aPosition);
       glBindBuffer(GL_ARRAY_BUFFER, planeCoord);
       safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
       
       safe_glEnableVertexAttribArray(h_aTexture);
       glBindBuffer(GL_ARRAY_BUFFER, textCoords[index]);
       safe_glVertexAttribPointer(h_aTexture, 2, GL_FLOAT, GL_FALSE, 0, 0);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    }
   
    void deactivate() {
       safe_glDisableVertexAttribArray(h_aPosition);
       safe_glDisableVertexAttribArray(h_aTexture);
       
       glDisable(GL_TEXTURE_2D);
    }
   
    void useTexture() {
       safe_glEnableVertexAttribArray(h_aTexture);
       glBindBuffer(GL_ARRAY_BUFFER, textCoords[0]);
       safe_glVertexAttribPointer(h_aTexture, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void setTexture(GLuint textureName) {
        GLint location = safe_glGetUniformLocation(m_shaderProg, "uTexUnit");
        safe_glUniform1i(location, textureName);
    }

	/*
    void setNormTexture(GLuint textureName) {
        GLint location = safe_glGetUniformLocation(m_shaderProg, "uNormTexUnit");
        safe_glUniform1i(location, textureName);
    }*/

    void setModelMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uModelMatrix, value_ptr(mat));
    }
   
    void setViewMatrix(mat4 mat) {
       safe_glUniformMatrix4fv(h_uViewMatrix, value_ptr(mat));
    }
   
    void setProjectionMatrix(mat4 mat) {
       safe_glUniformMatrix4fv(h_uProjMatrix, value_ptr(mat));
    }



private:
   
   void loadCoordinates(const char *filename)
   {
      ifstream in(filename, ios::in);
      if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
      
      string line;
      getline(in, line);
      istringstream n(line);
      n >> numCoords;
      
      names = new string[numCoords];
      textCoords = new GLuint[numCoords];
      
      for (int i = 0; i < numCoords && getline(in, line); i++) {
         istringstream s(line);
         s >> names[i];
         
         GLfloat coord[8];
         for (int j = 0; j < 8; j++) {
            s >> coord[j];
         }
         
         glGenBuffers(1, &textCoords[i]);
         glBindBuffer(GL_ARRAY_BUFFER, textCoords[i]);
         glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
      }
      
   }

   GLuint planeCoord;
   GLuint elements;
   GLuint textCoord;
   GLuint texture;
   // vertex
   GLint h_aPosition;
   GLint h_aTexture;

   // matrices
   GLint h_uModelMatrix;
   GLint h_uProjMatrix;
   GLint h_uViewMatrix;
   
   int numCoords;
   GLuint *textCoords;
   string *names;
};

#endif



