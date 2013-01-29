/*
 * GameObject.h
 * Author: Chris Pauley & Bryan Bell
 * 
 * Date: 1/28/2013
 * This class defines the most basic game object
 *  object has position, orientation, scale, and
 *  velocity
 */
 
#ifndef GAME_OBJECT
#define GAME_OBJECT

/* PhysX Object */
#include "glm/glm.hpp"

using namespace glm;

class GameObject {
public:
	GameObject();
	
	glm::vec3 direction() { return vec3(dir.x, dir.y, dir.z); };
	void setDirection(vec3 d) { dir = vec3(d.x, d.y, d.z); };
	float speed() { return spd; };
	void setSpeed(float s) { spd = s; }
	glm::vec3 position() { return vec3(pos.x, pos.y, pos.z); };
	void setPosition(vec3 p) { pos = vec3(p.x, p.y, p.z); };
	glm::vec3 rotation() { return vec3(rot.x, rot.y, rot.z); };
	void setRotation(vec3 r) { rot = vec3(r.x, r.y, r.z); };
	glm::vec3 scale() { return vec3(scl.x, scl.y, scl.z); };
	void setScale(vec3 s) { scl = vec3(s.x, s.y, s.z); };

protected:
	glm::vec3 dir;
	float spd;
	
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scl;	
};

#endif