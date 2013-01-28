
#ifndef MESH_H
#define MESH_H

#include "drawable.h"


class Mesh : public Drawable {

public:

    Mesh(std::string);

    bool init(std::string);

    void render();

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
