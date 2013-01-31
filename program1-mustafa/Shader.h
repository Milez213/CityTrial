/* 
 * Shader object
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <GL/glut.h>
#include "GLSL_helper.h"





class Shader {

public:

    Shader(const char *vertFname, const char *fragFname);
    ~Shader();

    void use();
    void disable();

    virtual void setModelMatrix(glm::mat4 m) = 0;


// for public inheritance
protected:
    GLuint m_shaderProg;
    bool m_installed;

private:


};

#endif



