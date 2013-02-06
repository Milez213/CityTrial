/* ------------------------------------------------------

 View Frustum - Lighthouse3D

  -----------------------------------------------------*/


#ifndef _FRUSTUMG_
#define _FRUSTUMG_

/*#ifndef _VEC3_
#include "Vec3.h"
#endif

class Vec3;
*/
#ifndef _PLANE_
#include "Plane.h"
#endif

class Plane;

/*#ifndef _AABOX_
#include "AABox.h"
#endif

class AABox;
*/

#include "glm/glm.hpp"

class FrustumG 
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};


public:

	static enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];


	Vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	FrustumG::FrustumG();
	FrustumG::~FrustumG();

	void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD);
	void FrustumG::setCamDef(vec3 &p, vec3 &l, vec3 &u);
	bool FrustumG::pointInFrustum(vec3 &p);
	//int FrustumG::sphereInFrustum(vec3 &p, float raio);
	//int FrustumG::boxInFrustum(AABox &b);


};


#endif
