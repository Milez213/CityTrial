#include "KPPDrawnObject.h"

class KPPDrawnObject : public KPPObject {

public: 
KPPDrawnObject(KPPPhysics *physics,const char *meshFile){
   
}


protected:
Mesh *m;
/*GLuint meshIndex;
GLunint textureIndex;
glm::vec3 diffColor;
glm::vec3 specColor;
float specularity;
KPPPhysics *p;
*/

void draw(Mesh *mesh,FlatShader *meshShader){
   
    meshShader->use();
    meshShader->setModelMatrix(position * scale * rotation);
    mesh->render(meshShader);

}

};
