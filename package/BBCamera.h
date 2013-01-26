//
// BBCamera.h
// Progam 01
//
// Created by Bryan Bell on 1/12/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines a basic game camera.

#ifndef BB_OBJECT
#define BB_OBJECT
#include "BBObject.h"
#endif

using namespace std;
using namespace glm;

class BBCamera : public BBObject
{
public:
	BBCamera();
	
	//Getters and Setters
	vec3 LookAt() { return vec3(lookAt.x, lookAt.y, lookAt.z); };
	void SetLookAt(vec3 lkt) {lookAt = vec3(lkt.x, lkt.y, lkt.z); };

private:
	vec3 lookAt;
};