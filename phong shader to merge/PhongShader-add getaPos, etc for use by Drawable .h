#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "defines.h"

using glm::vec3;
using glm::mat4;
using glm::value_ptr;

struct PhongMaterial {
  vec3 aColor;
  vec3 dColor;
  vec3 sColor;
  GLfloat shine;
};

struct LightInfo {
    vec3 pos;
    vec3 color;
};

class PhongShader : public Shader {

public:

    PhongShader();

    void setShowNormals(int show) {
        safe_glUniform1i(h_uShowNormals, show);
    }

    void setMaterial(PhongMaterial &mat) {
        safe_glUniform3f(h_uMatAmb, mat.aColor);
        safe_glUniform3f(h_uMatDif, mat.dColor);
        safe_glUniform3f(h_uMatSpec, mat.sColor);
        safe_glUniform1f(h_uMatShine, mat.shine);
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
    }

    void setViewMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uViewMatrix, value_ptr(mat));
    }

    void setProjMatrix(mat4 mat) {
        safe_glUniformMatrix4fv(h_uProjMatrix, value_ptr(mat));
    }


private:

    // vertex
    // TODO - not used
    GLint h_aPosition;
    GLint h_aColor;
    GLint h_aNormal;

    // matrices
    GLint h_uModelMatrix;
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



