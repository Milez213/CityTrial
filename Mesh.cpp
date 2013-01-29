 
#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl3.h>
#endif


#ifdef __unix__
#include <GL/glu.h>
#endif

#include "CMeshLoaderSimple.h"

#include "Mesh.h"



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

// assumes M V P matrices are set
// Renders using Vertex Array Object
void Mesh::draw() {
    glBindVertexArray(meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, m_TriangleCount * 3);
    glBindVertexArray(0);
}

// assumes M V P matrices are set
// render using a given shader's Attribute Position (gets h_aPosition)
void Mesh::draw(FlatShader *flatShader) {

   GLint h_aPos = flatShader->getPosLocation();

   safe_glEnableVertexAttribArray(h_aPos);
   glBindBuffer(GL_ARRAY_BUFFER, triBuffObj);

   safe_glVertexAttribPointer(h_aPos, 4, GL_FLOAT, GL_FALSE, 0, 0);
   glDrawArrays(GL_TRIANGLES, 0, m_TriangleCount * 3);
   safe_glEnableVertexAttribArray(flatShader->getPosLocation());

}



