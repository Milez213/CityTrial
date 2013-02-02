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


class GameObject {
public:
	GameObject();
	
   virtual void collide(GameObject *other);
   virtual void update(float dt);
   
   glm::vec3 velocity() { return vel; }
   
	glm::vec3 direction() { return vel; };
	void setDirection(glm::vec3 d) { vel = d; };
   //float direction();
   //void setDirection(float d);
	float speed();// { return spd; };
	void setSpeed(float s);// { spd = s; }
	glm::vec3 position() { return pos; };
	void setPosition(glm::vec3 p) { pos = p; };
   
	glm::vec3 rotation() { return rot; };
	void setRotation(glm::vec3 r) { rot = r; };
	glm::vec3 scale() { return scl; };
	void setScale(glm::vec3 s) { scl = s; };
   
	char *getName() { return name; };

protected:
   char *name;
   
	//glm::vec3 dir;
	//float spd;
   glm::vec3 vel;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scl;	
};

#endif