/*
 * GameObject.cpp
 * Author: Chris Pauley & Bryan Bell
 * 
 * Date: 1/28/2013
 * This class defines the most basic game object
 *  object has position, orientation, scale, and
 *  velocity
 */
 
#include "KPPObject.h"

GameObject::GameObject()
{
	
}

glm::vec3 velocity(){
   return velocity;
}
void set_velocity(glm::vec3 vel){
   velocity = vel;
}
glm::vec3 position(){
   return position;
}
void set_position(glm::vec3 p){
   position = p;
}
glm::vec3 rotation(){
   return rotation;
}
void set_rotation(glm::vec3 r){
   rotation = r;
}
glm::vec3 scale(){
   return scale;
}
void set_scale(glm::vec3 s){
   scale = s;
}
bound* bBox()
{
   return bBox:
}

void bBox(bound* b){
   bBox = b;
}

//Bounding Box?
//Bounding sphere?
/* Getters and Setters */
/* Velocity - vec3
* Position - vec3
* Rotation - vec3
* Scale - vec3
* Bounding Box*
* Bounding Sphere*
* MakeSure
*/
};

