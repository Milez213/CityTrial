#include "PhongShader.h"
#include <stdlib.h>


PhongShader::PhongShader()
    : Shader("mesh_vert.glsl", "mesh_frag.glsl") {
    

    if (m_installed) {
        // matrices
        h_uProjMatrix = safe_glGetUniformLocation(m_shaderProg, "uProjMatrix");
        h_uViewMatrix = safe_glGetUniformLocation(m_shaderProg, "uViewMatrix");
        h_uModelMatrix = safe_glGetUniformLocation(m_shaderProg, "uModelMatrix");

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
        fprintf(stderr, "FlatShader(): Couldn't install shaders\n");
        exit(1);
    }
}


