// From GLSL 4.0 cookbook


#ifndef VBOTORUS_H
#define VBOTORUS_H

#include <GL/glut.h>
#include "drawable.h"

class VBOTorus : public Drawable
{
private:
    unsigned int vaoHandle;
    int faces, rings, sides;
    
    GLuint handle[3];

    void generateVerts(float * , float * ,float *, unsigned int *,
                       float , float);

public:
    VBOTorus(float, float, int, int);

    void render();
    unsigned int getVertexArrayHandle() const;
};

#endif // VBOTORUS_H
