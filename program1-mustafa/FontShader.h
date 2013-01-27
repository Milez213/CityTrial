#ifndef FONT_SHADER_H
#define FONT_SHADER_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "defines.h"

#include <stdlib.h>


using glm::vec3;
using glm::mat4;
using glm::value_ptr;


class FontShader : public Shader {

public:

    FontShader()
    : Shader("font_vert.glsl", "font_frag.glsl") {
        if (m_installed) {
            h_uColor = safe_glGetUniformLocation(m_shaderProg, "color");
            h_aCoord = safe_glGetAttribLocation(m_shaderProg, "coord");

        } else {
            exit(1);
        }
    }

    void setModelMatrix(glm::mat4 m) {
    
    }



    void setTexture(GLuint textureName) {
        GLint location = safe_glGetUniformLocation(m_shaderProg, "tex");
        safe_glUniform1i(location, textureName);
    }
        
      
    void setColor(vec3 color) {
        safe_glUniform3f(h_uColor, color);
    }
    
    GLint getAtribCoord() {
        return h_aCoord;
    }



private:

    GLint h_aCoord;
    GLint h_uTex;
    GLint h_uColor;


};

#endif



