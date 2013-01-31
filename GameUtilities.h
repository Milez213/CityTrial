//
//  GameUtilities.h
//  
//
//  Created by Eric Johnson on 1/30/13.
//
//

#ifndef _GameUtilities_h
#define _GameUtilities_h

#include "glm/glm.hpp"
#include "PxPhysicsAPI.h"

glm::vec3 convert(const physx::PxVec3 &vec);
physx::PxVec3 convert(const glm::vec3 &vec);

#endif
