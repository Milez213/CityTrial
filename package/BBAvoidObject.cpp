//
// BBAvoidObject.cpp
// Progam 01
//
// Created by Bryan Bell on 1/14/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines an enemy object.

#include "BBAvoidObject.h"
#include <cstdlib>

BBAvoidObject::BBAvoidObject()
{
	srandom(time(NULL));
	int flag = random() % 100;
	
	if (flag < 50)
		velocity = vec4(0.0, 0.0, 1.0, 5.0);	
	else
		velocity = vec4(1.0, 0.0, 0.0, 5.0);	
}

BBAvoidObject::BBAvoidObject(BBMatObject object)
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
	
	scale = object.Scale();
	position = object.Position();
	rotation = object.Rotation();
	
	srandom(time(NULL));
	int flag = random() % 100;
	
	if (flag < 50)
		velocity = vec4(0.0, 0.0, 1.0, 10.0);	
	else
		velocity = vec4(1.0, 0.0, 0.0, 10.0);
}

void BBAvoidObject::SetVelocity(vec4 vel)
{
	vec3 direction = vec3(vel.x, vel.y, vel.z);
	direction = normalize(direction);
	velocity = vec4(direction.x, direction.y, direction.z, vel.w);	
}

#define ROT_SPEED 45
void BBAvoidObject::BBMove(double time)
{
	//printf("Position (%0.3f, %0.3f)\n", position.x, position.z);
	position.x += velocity.x * velocity.w * time;
	position.z += velocity.z * velocity.w * time;
	
	if (position.x > 50.0) {
		position.x = 50.0;
		velocity.x *= -1;
	} else if (position.x < -50.0) {
		position.x = -50.0;
		velocity.x *= -1;
	}
	
	if (position.z > 50.0) {
		position.z = 50.0;
		velocity.z *= -1;
	} else if (position.z < -50.0) {
		position.z = -50.0;
		velocity.z *= -1;
	}
}

void BBAvoidObject::BBRandomPosition()
{
	srandom(time(0));
	int xPos = (random() % 100) - 50;
	int zPos = (random() % 100) - 50;
	
	position.x = (float)xPos;
	position.y = 1.0;
	position.z = (float)zPos;
	
	int flag = random() % 100;
	
	if (flag < 50)
		velocity = vec4(0.0, 0.0, 1.0, 10.0);	
	else
		velocity = vec4(1.0, 0.0, 0.0, 10.0);	
}

void BBAvoidObject::BBCollideWithSphere(BBMatObject collide)
{	
	vec4 cCen = collide.Center();
	vec3 cPos = vec3(cCen.x, cCen.y, cCen.z);
	vec4 poso = Center();
	vec3 pos = vec3(poso.x, poso.y, poso.z);
	vec3 diff = pos - cPos;
	
	vec3 vel = vec3(velocity.x, velocity.y, velocity.z);
	
	vec3 newVel = normalize(2 * dot(vel, diff) * diff - vel);
	velocity.x = -newVel.x;
	velocity.y = -newVel.y;
	velocity.z = -newVel.z;
	
	diff = normalize(diff);
	
	position.x = cCen.x + diff.x * (cCen.w + poso.w) * 1.1;
	position.z = cCen.z + diff.z * (cCen.w + poso.w) * 1.1;
}

void BBAvoidObject::BBCollideWithBoundingBox(BBMatObject collide)
{
	
}