 
#include <stdio.h>
#include <GL/glut.h>

#include "CMeshLoaderSimple.h"
#include "mesh.h"

// for attrib locations
#include "defines.h"

Mesh::Mesh(std::string fname) {
    init(fname);
}

bool Mesh::init(std::string fname) {
    bool loaded;

    loaded = CMeshLoader::loadVBOFromMesh(fname, m_TriangleCount,
                triBuffObj,
                normPFBuffObj,
                normPVBuffObj,
                colBuffObj);

    if (!loaded) {
        perror(fname.c_str());
        return false;
    }

    cNormBuffObj = normPFBuffObj;

    // setup mesh vertex array
    glGenVertexArrays(1, &meshVAO);
    glBindVertexArray(meshVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, triBuffObj);
    glVertexAttribPointer(POS_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(POS_LOCATION);

    glBindBuffer(GL_ARRAY_BUFFER, cNormBuffObj);
    glVertexAttribPointer(NORM_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(NORM_LOCATION);

    glBindVertexArray(0);

    return true;
}

void Mesh::render() {
    glBindVertexArray(meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_TriangleCount * 3);
    glBindVertexArray(0);
}




