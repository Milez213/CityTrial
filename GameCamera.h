#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H


#include "GameObject.h"
#include <cmath>

#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc



class GameCamera : public GameObject {
protected:

    GameCamera() {
        m_up = glm::vec3(0, 1, 0);
    }


    glm::vec3 getLookAtTarget() {
        return m_lookAtTarget;
    }

    void setLookAtTarget(glm::vec3 l) {
        m_lookAtTarget = l;
    }

    // center is lookAt
    // angle in degrees
    void rotateLR(float angle) {
        glm::mat4 rotMat(1.0);
        rotMat = glm::rotate(glm::mat4(1.0), angle,
            glm::vec3(m_lookAtTarget.x, 1.0, m_lookAtTarget.z));

        glm::vec4 pos4 = rotMat * glm::vec4(pos.x, pos.y, pos.z, 1);
        pos.x = pos4.x;
        pos.y = pos4.y;
        pos.z = pos4.z;
    }

    glm::mat4 getViewMat() {
        return glm::lookAt(pos, m_lookAtTarget, m_up);
    }




private:

    // Camera is looking at this
    glm::vec3 m_lookAtTarget;
    glm::vec3 m_up;

};



#endif
