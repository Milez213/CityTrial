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

GameObject::GameObject(GamePhysicsActor *actor) : mActor(actor)
{
	//dir = vec3(0.0, 0.0, 1.0);
   //spd = 0.0;
   
   //pos = vec3(0.0, 0.0, 0.0);
   rot = vec3(0.0, 0.0, 0.0);
   scl = vec3(0.0, 0.0, 0.0);
}

glm::vec3 GameObject::velocity() { return mActor->velocity(); }
glm::vec3 GameObject::direction() { return mActor->direction(); }
void GameObject::setDirection(vec3 d) { mActor->setDirection(d); }
//float GameObject::direction() { return mActor->direction(); }
//void GameObject::setDirection(float d) { mActor->setDirection(d); }
float GameObject::speed() { return mActor->speed(); }
void GameObject::setSpeed(float s) { mActor->setSpeed(s); }
glm::vec3 GameObject::position() { return mActor->position(); }
void GameObject::setPosition(vec3 p) { mActor->setPosition(p); }