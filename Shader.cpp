
#include "Shader.h"


Shader::Shader(const char *vertFname, const char *fragFname) {
    m_installed = false;
    m_installed = installShader(vertFname,
                                fragFname,
                                m_shaderProg);
}


void Shader::use() {
    glUseProgram(m_shaderProg);
}

void Shader::disable() {
    glUseProgram(0);
}

Shader::~Shader() {
    if (m_shaderProg != 0) {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
}

