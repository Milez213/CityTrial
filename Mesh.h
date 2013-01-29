
#ifndef MESH_H
#define MESH_H

#include "Drawable.h"

#include "FlatShader.h"

class Mesh : public Drawable {

public:

    Mesh(std::string);

    bool init(std::string);

	// assumes M V P matrices are set
    void draw();
    void draw(FlatShader *);

    int getTriangleCount() {
        return m_TriangleCount;
    }


private:
    GLuint triBuffObj, colBuffObj, normPFBuffObj,
     normPVBuffObj, cNormBuffObj;
    GLuint meshVAO;
    int m_TriangleCount;


};

#endif
