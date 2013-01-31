//
// BBLight.h
// Progam 01
//
// Created by Bryan Bell on 1/12/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines a light object.

#ifndef BB_OBJECT
#define BB_OBJECT
#include "BBObject.h"
#endif

using namespace std;
using namespace glm;

class BBLight : public BBObject
{
public:
	BBLight();

// Getters and Setters
	vec3 Color() { return vec3(color.x, color.y, color.z); };
	void SetColor(vec3 col) { color = vec3(col.x, col.y, col.z); };
	float Intensity() { return intensity; };
	void SetIntensity(float itn) { intensity = itn; };
	
protected:
	vec3 color;
	float intensity;
};