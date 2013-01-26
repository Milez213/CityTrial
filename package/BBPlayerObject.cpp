//
// BBPlayerObject.cpp
// Progam 01
//
// Created by Bryan Bell on 1/14/13
// For CSC 476 - Real Time 3D Graphics
//
// This object defines a player controlled object.

#include "BBPlayerObject.h"

BBPlayerObject::BBPlayerObject()
{
	velocity = vec4(0.0, 0.0, 1.0, 1.0);	
	points = 0;
}

BBPlayerObject::BBPlayerObject(BBMatObject object)
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
	
	velocity = vec4(0.0, 0.0, 0.0, 10.0);
	points = 0;	
}

void BBPlayerObject::SetVelocity(vec4 vel)
{
	vec3 direction = vec3(vel.x, vel.y, vel.z);
	direction = normalize(direction);
	velocity = vec4(direction.x, direction.y, direction.z, vel.w);	
}

#define ROT_SPEED 45
void BBPlayerObject::BBMove(double time)
{
	position += vec3(velocity.x * time, velocity.y * time, velocity.z * time);
}

void BBPlayerObject::BBReceiveJoystick(float axes[], vec3 gaze, vec3 u)
{
	vec2 g2 = normalize(vec2(gaze.x, gaze.z)) * -axes[3] * velocity.w;
	vec2 u2 = normalize(vec2(u.x, u.z)) * axes[2]  * velocity.w;
	
	//printf("g2: (%0.3f, %0.3f) - u2: (%0.3f, %0.3f)\n", g2.x, g2.y, u2.x, u2.y);
	velocity.z = (u2.y + g2.y);
   velocity.x = (u2.x + g2.x);
   //printf("Velocity: (%0.3f, %0.3f)\n", velocity.x, velocity.z);
}

void BBPlayerObject::BBButtonPressed(char button)
{
	
}

void BBPlayerObject::BBMouseMoved(vec2 distance)
{
	
}

void BBPlayerObject::BBCollideWithSphere(BBMatObject collide)
{
	if (collide.ShaderFlags().w == 1.0) {
		points += 100;
		printf("Player Points: %d\n", points);
	} else {
		points = 0;
		printf("Oh No!!!\n");
	}
}

void BBPlayerObject::BBCollideWithBoundingBox(BBMatObject collide)
{
	
}

