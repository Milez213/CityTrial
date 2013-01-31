#include "KPPObject.h"


class KPPCamera : public KPPObject {
  protected:
	        glm::vec3 lookAt();
	        void set_lookAt(glm::vec3 l);
};

