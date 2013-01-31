//
// BBPlayerObject.h
// Progam 01
//
// Created by Bryan Bell on 1/14/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines a player controlled object.

#ifndef BB_MAT_OBJECT
#define BB_MAT_OBJECT
#include "BBMatObject.h"
#endif

class BBPlayerObject : public BBMatObject
{
public:
	BBPlayerObject();
	BBPlayerObject(BBMatObject object);
	
	void BBMove(double time);
	void BBButtonPressed(char button);
	void BBMouseMoved(vec2 distance);
	void BBReceiveJoystick(float axes[], vec3 gaze, vec3 u);
	
	void BBCollideWithSphere(BBMatObject collide);
	void BBCollideWithBoundingBox(BBMatObject collide);
	
	//GETTERS AND SETTERS
	vec4 Velocity() { return velocity; };
	void SetVelocity(vec4 vel); //Normalize needed; defined in .cpp
	int GetPoints() { return points; };
	
protected:
	vec4 velocity;
	int points;
};