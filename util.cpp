
#include "util.h"


#include "defines.h"

#include <cmath>
#include <stdlib.h>
#include <sstream>


// x, z -> new x and z rotated theta around y axis
// theta in degrees
void rotate2d(float &x, float &z, const float theta) {
   float x1, z1;
   const float cos_th = cos(TO_RADIANS(theta));
   const float sin_th = sin(TO_RADIANS(theta));
   x1 = x * cos_th - z * sin_th;
   z1 = x * sin_th + z * cos_th;
   x = x1;
   z = z1;
}

// returns in range -1, 1 (not sure if incusive)
float randFloat() {
       return ((float) rand() / RAND_MAX);
}


float vecAngle(glm::vec3 v1, glm::vec3 v2) {
   return glm::degrees(glm::acos(glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2))));
}

float absAngleDif(float a1, float a2) {
   return glm::degrees(glm::acos(glm::cos(glm::radians(a1-a2))));
}


std::string toString(int i)
{
   return static_cast<std::ostringstream*>( &(std::ostringstream() << i) )->str();
}