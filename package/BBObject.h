//
// BBObject.h
// Progam 01
//
// Created by Bryan Bell on 1/12/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines a basic game object.

#include <stdlib.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;
using namespace glm;

class BBObject
{
public:
	BBObject();

// Getters and Setters
	vec3 Scale() { return vec3(scale.x, scale.y, scale.z); };
	void SetScale(vec3 scl) { scale = vec3(scl.x, scl.y, scl.z); }; 
	vec3 Position() { return vec3(position.x, position.y, position.z); };
	void SetPosition(vec3 pos) { position = vec3(pos.x, pos.y, pos.z); };
	vec3 Rotation() { return vec3(rotation.x, rotation.y, rotation.z); };
	void SetRotation(vec3 rot) { rotation = vec3(rot.x, rot.y, rot.z); };
	
protected:
	vec3 scale;
	vec3 position;
	vec3 rotation;
};