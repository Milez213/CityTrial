#include "KPPCamera.h"
class KPPCamera : public KPPObject {
protected:
glm::vec3 lookAt;

glm::vec3 lookAt(){
   return lookAt;
}
void set_lookAt(glm::vec3 l){
   lookAt = l;
}
/* Getters and Setters */
/* LookAt - vec3 */
};
