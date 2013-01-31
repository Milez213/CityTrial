#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H

#include "GameObject.h"


class GameLight : public GameObject {


public:

    GameLight(vec3 pos, vec3 color) {
        this->pos = pos;
        this->color = color;
    }
    
    vec3 get_color(){
       return color;
    }
    
    void set_color(glm::vec3 c){
       color = c;
    }


    vec3 get_intensity(){
       return intensity;
    }
    
    void set_intensity(glm::vec3 i){
       intensity = i;
    }
    
protected:
    vec3 color;
    vec3 intensity;
    
};

#endif


