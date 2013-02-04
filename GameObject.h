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
	
   virtual void update(float dt);
   
   virtual glm::vec3 getVelocity() { return vel; }
   
	
	virtual glm::vec3 getPosition() { return pos; };
	virtual void setPosition(glm::vec3 p) { pos = p; };
   
	virtual glm::vec3 getRotation() { return rot; };
	virtual void setRotation(glm::vec3 r) { rot = r; };
	virtual glm::vec3 getScale() { return scl; };
	virtual void setScale(glm::vec3 s) { scl = s; };
   
	char *getName() { return name; };
    void setName(const char *new_name) {
        // TODO - use c++ string or strncpy?
        name = (char*) new_name;
    }

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
