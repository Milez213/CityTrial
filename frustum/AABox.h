/* ------------------------------------------------------

 Axis Aligned Boxes - Lighthouse3D

  -----------------------------------------------------*/


#ifndef _AABOX_
#define _AABOX_

#ifndef _vec3_
#include "vec3.h"
#endif

class vec3;

class AABox 
{

public:

	vec3 corner;
	float x,y,z;


	AABox::AABox( vec3 &corner, float x, float y, float z);
	AABox::AABox(void);
	AABox::~AABox();

	void AABox::setBox( vec3 &corner, float x, float y, float z);

	// for use in frustum computations
	vec3 AABox::getVertexP(vec3 &normal);
	vec3 AABox::getVertexN(vec3 &normal);


};


#endif
