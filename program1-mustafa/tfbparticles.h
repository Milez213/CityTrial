// ideas from GLSL 4.0 cookbook

#ifndef TFB_PARTICLES_H
#define TFB_PARTICLES_H


#include <GL/glut.h>
#include <math.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "GLSL_helper.h"
#include "defines.h"
#include "drawable.h"


// so it will compile on CSL
#ifdef CSL
#ifndef glGenTransformFeedbacks 
#define glGenTransformFeedbacks glGenTransformFeedbacksNV
#endif

#ifndef glBindTransformFeedback 
#define glBindTransformFeedback glBindTransformFeedbackNV
#endif

#ifndef GL_TRANSFORM_FEEDBACK 
#define GL_TRANSFORM_FEEDBACK GL_TRANSFORM_FEEDBACK_NV
#endif

#endif // CSL


using glm::mat4;
using glm::vec3;
using glm::value_ptr;
using namespace std;


class TFBParticles : public Drawable {
    public:
        int id;

        TFBParticles();
        TFBParticles(float time);
        TFBParticles(float time, float spread, float iVel);

        ~TFBParticles() {
            glDeleteShader(m_shaderProg);
        };

        bool init(vec3 p0);
        void update(float t);
        void render();

        void setModelMatrix(mat4 mat);
        void setViewMatrix(mat4 mat);
        void setProjMatrix(mat4 mat);

        void use();
        void disable();

        void stop(); // stop recycling

        void setPos(vec3 &p1) {
            use();
            Pos0 = Pos1;
            Pos1 = p1;
            setUniform("Pos0", Pos1);
            // print3f(Pos1, "Pos0");
        }

        void setVel(vec3 v) {
            use();
            setUniform("Vel0", v);
        }

    private:

        // GL object handles
        GLuint m_shaderProg;
        GLuint m_drawShader;

        // particle buffers
        GLuint posBuf[2], velBuf[2], startTime[2], initVel;
        GLuint particleArray[2]; // GL container
        GLuint feedback[2]; 
        GLuint drawBuf, query;

        GLuint particleTex;

        float t;
        float dt;
        float life;
        int nParticles;
        int live;


        vec3 Pos0;
        vec3 Pos1;

        float m_spread;
        float m_iVel;

        float randFloat();
        
        bool installTFShader();
        bool initBuffers();


        void setTexture(GLuint textureName);

        void setUniform(const char *name, const mat4 mat);
        void setUniform(const char *name, const vec3 v);
        void setUniform(const char *name, const float val);
        void setUniform(const char *name, const GLint val);

        // experimental
        void drawParticles();
};

#endif
