//
//  GameUtilities.cpp
//  
//
//  Created by Eric Johnson on 1/30/13.
//
//

#include "GameUtilities.h"

glm::vec3 convert(const physx::PxVec3 &vec)
{
   return glm::vec3(vec.x, vec.y, vec.z);
}
physx::PxVec3 convert(const glm::vec3 &vec)
{
   return physx::PxVec3(vec.x, vec.y, vec.z);
}