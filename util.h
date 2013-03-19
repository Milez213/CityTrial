#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H

#include <string>
#include "glm/glm.hpp"

// x, z -> new x and z rotated theta around y axis
// theta in degrees
void rotate2d(float &x, float &z, const float theta);

// -1, 1. doesn't seed
float randFloat();

float vecAngle(glm::vec3 v1, glm::vec3 v2);

float absAngleDif(float a1, float a2);

std::string toString(int);


#endif
