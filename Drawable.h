#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "BaseObject.h"

#include "include_glm.h"

// Abstract Class
class Drawable : public BaseObject {
public:
    Drawable() {
    }
    
    virtual void draw() = 0;
    
    vec3 pos;
    vec3 rot;
    vec3 scaleVec;
    
    // TODO - getters and setters?
    
};


#endif // DRAWABLE_H
