#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "defines.h"

#include <stdlib.h>

using glm::vec3;
using glm::mat4;
using glm::value_ptr;

struct PhongMaterial {
   vec3 aColor;
   vec3 dColor;
   vec3 sColor;
   GLfloat shine;
   
   GLuint textureLocation;
   GLuint textureCoordinates;
};

struct LightInfo {
   vec3 pos;
   vec3 color;
};

class PhongShader : public Shader {

public:

    PhongShader() : Shader("shaders/phong_vert.glsl", "shaders/phong_frag.glsl") {
        if (m_installed) {
            // matrices
            h_uProjMatrix = safe_glGetUniformLocation(m_shaderProg, "uProjMatrix");
            h_uViewMatrix = safe_glGetUniformLocation(m_shaderProg, "uViewMatrix");
            h_uModelMatrix = safe_glGetUniformLocation(m_shaderProg, "uModelMatrix");
            h_uiModelMatrix = safe_glGetUniformLocation(m_shaderProg, "uiModelMatrix");

            // attributes
            h_aPosition = safe_glGetAttribLocation(m_shaderProg, "aPosition");
            h_aNormal = safe_glGetAttribLocation(m_shaderProg, "aNormal");

            // TODO - add texture coordinates handle in shader

            // light  stuff
            h_uLightPos = safe_glGetUniformLocation(m_shaderProg, "uLightPos");
            h_uLightColor = safe_glGetUniformLocation(m_shaderProg, "uLColor");
            h_uMatAmb = safe_glGetUniformLocation(m_shaderProg, "uMat.aColor");
            h_uMatDif = safe_glGetUniformLocation(m_shaderProg, "uMat.dColor");
            h_uMatSpec = safe_glGetUniformLocation(m_shaderProg, "uMat.sColor");
            h_uMatShine = safe_glGetUniformLocation(m_shaderProg, "uMat.shine");
            h_uShowNormals = safe_glGetUniformLocation(m_shaderProg, "uShowNormals");
            h_uCamPos = safe_glGetUniformLocation(m_shaderProg, "uCamPos");
        } else {
            fprintf(stderr, "PhongShader(): Couldn't install shaders\n");
            exit(1);
        }
    }

    void setShowNormals(int show) {
        safe_glUniform1i(h_uShowNormals, show);
    }

    GLuint getNormLocation() {
        return h_aNormal;
    }

    GLuint getPosLocation() {
        return h_aPosition;
    }

    void setMaterial(PhongMaterial *mat) {
        safe_glUniform3f(h_uMatAmb, mat->aColor);
        safe_glUniform3f(h_uMatDif, mat->dColor);
        safe_glUniform3f(h_uMatSpec, mat->sColor);
        safe_glUniform1f(h_uMatShine, mat->shine);
    }

    void setLight(const LightInfo &l) {
        safe_glUniform3f(h_uLightPos, l.pos);
        safe_glUniform3f(h_uLightColor, l.color);
    }

    void setCamPos(vec3 pos) {
        safe_glUniform3f(h_uCamPos, pos);
    }

    void setTexture(GLuint textureName) {
        GLint location = safe_glGetUniformLocation(m_shaderProg, "uTexUnit");
        safe_glUniform1i(location, textureName);
    }

    void setNormTexture(GLuint textureName) {
        GLint location = safe_glGetUniformLocation(m_shaderProg, "uNormTexUnit");
        safe_glUniform1i(location, textureName);
    }

    void setModelMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uModelMatrix, value_ptr(mat));
        setiModelMatrix(mat);
    }

    void setiModelMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uiModelMatrix, value_ptr(mat));
    }

    void setViewMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uViewMatrix, value_ptr(mat));
    }

    void setProjMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uProjMatrix, value_ptr(mat));
    }


private:

    // vertex
    GLint h_aPosition;
    GLint h_aColor;
    GLint h_aNormal;

    // matrices
    GLint h_uModelMatrix;
    GLint h_uiModelMatrix;
    GLint h_uViewMatrix;
    GLint h_uProjMatrix;

    // light info
    GLint h_uLightPos;
    GLint h_uLightColor;
    GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;

    GLint h_uShowNormals;
    GLint h_uCamPos;


};

#endif



