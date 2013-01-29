/*
 * GameObject.cpp
 * Author: Chris Pauley & Bryan Bell
 * 
 * Date: 1/28/2013
 * This class defines the most basic game object
 *  object has position, orientation, scale, and
 *  velocity
 */
 
#include "GameObject.h"

GameObject::GameObject()
{
	dir = vec3(0.0, 0.0, 1.0);
   spd = 0.0;
   
   pos = vec3(0.0, 0.0, 0.0);
   rot = vec3(0.0, 0.0, 0.0);
   scl = vec3(0.0, 0.0, 0.0);
}
