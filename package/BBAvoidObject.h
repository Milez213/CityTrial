//
// BBAvoidObject.h
// Progam 01
//
// Created by Bryan Bell on 1/13/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines an enemy object.

#ifndef BB_MAT_OBJECT
#define BB_MAT_OBJECT
#include "BBMatObject.h"
#endif

class BBAvoidObject : public BBMatObject
{
public:
	BBAvoidObject();
	BBAvoidObject(BBMatObject object);
	
	void BBMove(double time);
	void BBRandomPosition();
	
	void BBCollideWithSphere(BBMatObject collide);
	void BBCollideWithBoundingBox(BBMatObject collide);
	
	//GETTERS AND SETTERS
	vec4 Velocity() { return velocity; };
	void SetVelocity(vec4 vel); //Normalize needed; defined in .cpp
	
protected:
	vec4 velocity;
};