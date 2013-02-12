#ifndef CALF_FRUSTUM_H
#define CALF_FRUSTUM_H

#include "include_glm.h"


void ExtractFrustum();

bool checkFrust(vec3 pos);

float SphereInFrustum( vec3 pos, float radius );


#endif
