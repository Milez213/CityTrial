#include "KPPObject.h"
#include "Mesh.h"
class KPPDrawnObject : public KPPObject {
         public: KPPDrawnObject(KPPPhysics *physics,const char *meshFile);
         protected:
	         void draw(/*Shader and Handlers*/);
};

