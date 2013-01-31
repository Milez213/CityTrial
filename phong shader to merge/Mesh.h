
#ifndef MESH_H
#define MESH_H


class Mesh {

public:

    Mesh(std::string);

    bool init(std::string);

    void draw();

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
