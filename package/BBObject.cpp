//
// BBObject.cpp
// Progam 01
//
// Created by Bryan Bell on 1/12/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines a basic game object.

#include "BBObject.h"

BBObject::BBObject()
{
	scale = vec3(0.0, 0.0, 0.0);
	position = vec3(0.0, 0.0, 0.0);
	rotation = vec3(0.0, 0.0, 0.0);	
}

