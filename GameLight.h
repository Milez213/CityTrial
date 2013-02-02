#include "GameDrawableObject.h"

#include "glm/glm.hpp"



class GameLight : public GameDrawableObject {

protected:

    // == Color ===
	glm::vec3 getColor(){
	   return color;
	}
	void setColor(glm::vec3 c){
	   color = c;
	}
	
    // == Intensity ===	
	glm::vec3 getIntensity(){
	   return intensity;
	}
	void setIntensity(glm::vec3 i){
	   intensity = i;
	}
	
	
	glm::vec3 color;
	glm::vec3 intensity;

	
};
