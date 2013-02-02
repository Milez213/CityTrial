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
#include <iostream>

GameObject::GameObject() : vel(0.0, 0.0, 1.0), pos(0.0), rot(0.0), scl(0.0)
{
   
}

void GameObject::collide(GameObject *other)
{
   
}

void GameObject::update(float dt)
{
   pos += dt * vel;
}


//glm::vec3 GameObject::velocity() { return mActor->velocity(); }
//glm::vec3 GameObject::direction() { return mActor->direction(); }
//void GameObject::setDirection(vec3 d) { mActor->setDirection(d); }
//float GameObject::direction() { return mActor->direction(); }
//void GameObject::setDirection(float d) { mActor->setDirection(d); }
float GameObject::speed() { return glm::length(vel); }
void GameObject::setSpeed(float s) { vel = s * glm::normalize(vel); }
//glm::vec3 GameObject::position() { return mActor->position(); }
//void GameObject::setPosition(vec3 p) { mActor->setPosition(p); }
