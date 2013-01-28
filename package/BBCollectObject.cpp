//
// BBAvoidObject.cpp
// Progam 01
//
// Created by Bryan Bell on 1/14/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines an collectable object.

#include "BBCollectObject.h"
#include <cstdlib>

#include <time.h>

BBCollectObject::BBCollectObject()
{
	timeSinceCaptured = -1.0;
}

BBCollectObject::BBCollectObject(BBMatObject object)
{
	vertexBuffer = object.VertexBuffer();
	normalBuffer = object.NormalBuffer();
	indexBuffer = object.IndexBuffer();
	textureBuffer = object.TextureBuffer();
	normalMapBuffer = object.NormalMapBuffer();
	indexBufferLength = object.IndexBufferLength();
	
	textureLocation = object.TextureLocation();
	
	center = object.Center();
	boundingBoxMax = object.BoundingBoxMax();
	boundingBoxMin = object.BoundingBoxMin();
	
	diffColor = object.DiffColor();
	diffuse = object.Diffuse();
	specColor = object.SpecColor();
	specularity = object.Specularity();
	
	shaderFlags = object.ShaderFlags();
	shaderFlags.w = 0.0;
	
	scale = object.Scale();
	position = vec3(0.0, 0.0, 0.0);
	rotation = vec3(0.0, 0.0, 0.0);
	
	timeSinceCaptured = -1.0;
}

void BBCollectObject::SetVelocity(vec4 vel)
{
	vec3 direction = vec3(vel.x, vel.y, vel.z);
	direction = normalize(direction);
	velocity = vec4(direction.x, direction.y, direction.z, vel.w);	
}

#define ROT_SPEED 45
void BBCollectObject::BBMove(double time)
{
	//printf("Position (%0.3f, %0.3f)\n", position.x, position.z);
	rotation.x += velocity.x * velocity.w * time;
	rotation.z += velocity.z * velocity.w * time;
	
	if (rotation.x > 360.0) {
		rotation.x -= 360.0;
	} else if (rotation.x < 0.0) {
		rotation.x += 360;
	}
	
	if (rotation.z > 360.0) {
		rotation.z -= 360.0;
	} else if (rotation.z < 0.0) {
		rotation.z += 360.0;
	}
	
	if (timeSinceCaptured > -1) {		
		position.y += 2.0 * time;	
	}
}

void BBCollectObject::BBRandomPosition()
{
	srandom(time(0));
	int xPos = (random() % 100) - 50;
	int zPos = (random() % 100) - 50;
	
	position.x = (float)xPos;
	position.y = 1.0;
	position.z = (float)zPos;
	
	rotation = vec3(0.0, 0.0, 0.0);
	
	timeSinceCaptured = -1.0;
	
	int flag = random() % 100;
	
	if (flag < 50)
		velocity = vec4(0.0, 0.0, 1.0, 180.0);	
	else
		velocity = vec4(1.0, 0.0, 0.0, 180.0);	
}

void BBCollectObject::BBCollideWithSphere(BBMatObject collide)
{
	timeSinceCaptured = 10.0;
	velocity.w *= 2;
	shaderFlags.w = 1000.0;
}

void BBCollectObject::BBCollideWithBoundingBox(BBMatObject collide)
{
	
}
