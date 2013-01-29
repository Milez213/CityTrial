/* 
 * Shader object
 *
 */

#ifndef SHADER_H
#define SHADER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#include "GLSL_helper.h"

class Shader {

public:

    Shader(const char *vertFname, const char *fragFname);
    ~Shader();

    void use();
    void disable();

// for public inheritance
protected:
    GLuint m_shaderProg;
    bool m_installed;

private:


};

#endif



