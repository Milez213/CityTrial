// Plane.cpp
//
//////////////////////////////////////////////////////////////////////

#include "Plane.h"
#include <stdio.h>


Plane::Plane( vec3 &v1,  vec3 &v2,  vec3 &v3) {

	set3Points(v1,v2,v3);
}


Plane::Plane() {}

Plane::~Plane() {}


void Plane::set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3) {


	vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = aux2 * aux1;

	normal = glm::normalize(normal);
	point.x = v2.x;
   point.y = v2.y;
   point.z = v2.z;

	d = -(normal.x*point.x + normal.y * point.y + normal.z * point.z);
}

void Plane::setNormalAndPoint(vec3 &normal, vec3 &point) {

	/*this->normal.copy(normal);
	this->normal.normalize();
	d = -(this->normal.innerProduct(point));*/
}

void Plane::setCoefficients(float a, float b, float c, float d) {
/*
	// set the normal vector
	normal.set(a,b,c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal.set(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d/l;*/
}


	

float Plane::distance(vec3 &p) {

   float c = normal.x*p.x + normal.y * p.y + normal.z * p.z;
	return (d + c);
}

void Plane::print() {

	printf("Plane(");
   //normal.print();printf("# %f)",d);
}
