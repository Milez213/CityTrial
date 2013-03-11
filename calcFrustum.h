#ifndef CALF_FRUSTUM_H
#define CALF_FRUSTUM_H

#include "include_glm.h"
#include "ModelManager.h"

void ExtractFrustum();

bool checkFrust(vec3 pos);

float SphereInFrustum( vec3 pos, float radius );

bool isBoundInFrustum(bound boundInfo);

#endif
