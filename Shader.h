/* 
 * Shader object
 *
 */

#ifndef SHADER_H
#define SHADER_H

#include <GL/gl.h>

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



