#include "KPPObject.h"
#include "Mesh.h"
#include "Drawable.h"
#include "FlatShader.h"

class KPPDrawnObject : public KPPObject {
         public: KPPDrawnObject(KPPPhysics *physics,const char *meshFile);
         protected:
         Mesh *m;
	         void draw(/*Shader and Handlers*/);
};

