#ifndef GAME_CAMERA_FP_H
#define GAME_CAMERA_FP_H


#include "GameObject.h"
#include <cmath>

#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc



class GameCameraFirstPerson : public GameObject {


public:

    GameCameraFirstPerson() {

    }


    // gets vector pointing from pos to object
    glm::vec3 getLookAt() {
        return lookAt;
    }


    // 1st person view
    glm::mat4 getViewMat() {
        using namespace glm;

        mat4 Vinv(1.0);

        // right-left rotate
        Vinv = rotate(mat4(1.0), angle.y, vec3(0.0f, 1, 0));

        Vinv = rotate(Vinv, angle.x, vec3(1, 0, 0));

        // get lookat vec
        vec4 t = Vinv * vec4(0, 0, -1, 1);
        lookAt.x = t.x;
        lookAt.y = t.y;
        lookAt.z = t.z;

        // S is translation with respect to Cam
        mat4 S = translate(mat4(1.0), camDeltaPos);

        mat4 V = inverse(Vinv);
        
        // S with respect to World
        vec4 camPos4f = vec4(pos.x, pos.y, pos.z, 1.0f);
        camPos4f = Vinv * S * V * camPos4f;
        // world coords pos
        pos.x = camPos4f.x;
        pos.y = camPos4f.y;
        pos.z = camPos4f.z;

        // printf("g_CamPos: %f, %f, %f\n", g_CamPos.x, g_CamPos.y, g_CamPos.z);

        // xlate camera in world
        Vinv =  translate(mat4(1), pos) * Vinv;

        //g_CamDz = g_CamDx = 0;

        return inverse(Vinv);
    }


    void update(float dt, vec3 playerDir, float moveSpeed) {
        camDeltaPos = vec3(playerDir.x * dt * moveSpeed, 
                           playerDir.y * dt * moveSpeed,
                           playerDir.z * dt * moveSpeed);
    }

public:

    vec3 angle;

protected:



    // rotate camera left-right around object

    glm::vec3 camDeltaPos;

    glm::vec3 lookAt;

};



#endif
