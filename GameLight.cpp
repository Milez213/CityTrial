#include "KPPLight.h"

class KPPLight : public KPPObject {
protected:
glm::vec3 color;
glm::vec3 intensity;

glm::vec3 color(){
   return color;
}
void set_color(glm::vec3 c){
   color = c;
}
glm::vec3 intensity(){
   return intensity;
}
void set_intensity(glm::vec3 i){
   intensity = i;
}
/* Getters and Setters */
/* Color - vec3
* Intensity - float */
};
