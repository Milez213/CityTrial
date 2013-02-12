#ifndef UTIL_FUNCS_H
#define UTIL_FUNCS_H




// x, z -> new x and z rotated theta around y axis
// theta in degrees
void rotate2d(float &x, float &z, const float theta);

// -1, 1. doesn't seed
float randFloat();


#endif
