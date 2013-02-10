#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H


#include "GameObject.h"
#include <cmath>

#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc



class GameCamera : public GameObject {


public:

    GameCamera() {
        m_up = glm::vec3(0, 1, 0);
    }


    glm::vec3 getLookAtTarget() {
        return m_lookAtTarget;
    }

    glm::vec3 getUp() {
       return m_up;
    }

    void setLookAtTarget(glm::vec3 l) {
        m_lookAtTarget = l;
    }

    void setYaw(float angle) {
        rot.y = angle;
    }

    // center is lookAt
    // angle in degrees
    glm::mat4 getViewMat() {
        rotateLR(rot.y);
        return glm::lookAt(pos, m_lookAtTarget, m_up);
    }


protected:



    // rotate camera left-right around object
    void rotateLR(float angle) {
        glm::mat4 rotMat(1.0);
        rotMat = glm::rotate(glm::mat4(1.0), angle,
            glm::vec3(0, 1.0, 0));

        //rotate around m_lookAtTarget
        glm::vec4 pos4 = rotMat * glm::vec4(pos.x - m_lookAtTarget.x,
                                            pos.y - m_lookAtTarget.y,
                                            pos.z - m_lookAtTarget.z, 1);

        pos.x = pos4.x + m_lookAtTarget.x;
        pos.y = pos4.y + m_lookAtTarget.y;
        pos.z = pos4.z + m_lookAtTarget.z;
    }


    // Camera is looking at this
    glm::vec3 m_lookAtTarget;
    glm::vec3 m_up;

};



#endif
