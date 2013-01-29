#include "KPPObject.h"

class KPPLight : public KPPObject {
	protected:
	        glm::vec3 color();
	        void set_color(glm::vec3 c);
	        glm::vec3 intensity();
	        void set_intensity(glm::vec3 i);
		/* Getters and Setters */
		/* Color - vec3 
		 * Intensity - float */	
};
