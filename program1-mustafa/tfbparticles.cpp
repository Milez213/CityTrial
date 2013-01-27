#include "tfbparticles.h"
#include "stdio.h"
#include "LoadTexture.h"


// if created at beginning of program
TFBParticles::TFBParticles() {
    t = 0;
    m_spread = (float) PI/10.0f;
    m_iVel = 0.2;
}


// if created at some time
TFBParticles::TFBParticles(float time) {
    t = time;
    m_spread = (float) PI/10.0f;
    m_iVel = 0.05;

}


TFBParticles::TFBParticles(float time, float spread, float iVel) {
    t = time;
    m_spread = spread;
    m_iVel = iVel;
}



bool TFBParticles::init(vec3 p0) {
    dt = 0;
    drawBuf = 1;
    live = 1;

    // particle parameters
    nParticles = 100;
    life = 1.0f;

    Pos0 = p0;
    // Dir = dir;

    if (installTFShader() == false) {
        return false;
    }

    if (initBuffers() == false) {
        return false;
    }

    // glClearColor(0.1f,0.1f,0.1f,0.0f);
    glPointSize(3.0f);

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE);


    glGenQueries(1, &query);
    
    // TODO - add texture
    particleTex = 1;
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, particleTex);

    use();

    setUniform("ParticleLifetime", life);
    setUniform("Accel", vec3(0.0f, -0.0f,0.0f));
    setTexture(particleTex);
    
    return true;
}



bool TFBParticles::initBuffers() {
    // mostly from GLSL 4.0 cookbook
    
    // Generate the buffers
    glGenBuffers(2, posBuf);    // position buffers
    glGenBuffers(2, velBuf);    // velocity buffers
    glGenBuffers(2, startTime); // Start time buffers
    glGenBuffers(1, &initVel);  // Initial velocity buffer (never changes, only need one)

    // Allocate space for all buffers
    int size = nParticles * 3 * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);
    
    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);
    

    // Fill the first position buffer with zeroes
    GLfloat *data = new GLfloat[nParticles * 3];

    for (int i = 0; i < nParticles; i++) {
        data[3*i] = Pos0.x;
        data[3*i+1] = Pos0.y;
        data[3*i+2] = Pos0.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    
    // init velocity
    // Fill the first velocity buffer with random velocities
    vec3 v(0.0f);
    float velocity, theta, phi;
    for (int i = 0; i < nParticles; i++) {
        //theta = (float) PI / 12;
        theta = glm::mix(0.0f, m_spread, randFloat());
        phi = glm::mix(0.0f, (float)TWOPI, randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        
        velocity = glm::mix(m_iVel, m_iVel * 1.5f , randFloat());
        v = glm::normalize(v) * velocity;

        data[3*i]   = v.x;
        data[3*i+1] = v.y;
        data[3*i+2] = v.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER,velBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    // Fill the first start time buffer
    delete [] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = life/(nParticles);
    for(int i = 0; i < nParticles; i++ ) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    delete [] data;


    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particleArray);

    // Set up particle array 0
    glBindVertexArray(particleArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    // Set up particle array 1
    glBindVertexArray(particleArray[1]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    return true;
}

float TFBParticles::randFloat() {
    return ((float) rand() / RAND_MAX);
}



void TFBParticles::disable() {
    glUseProgram(0);
}

void TFBParticles::use() {
    glUseProgram(m_shaderProg);
}

void TFBParticles::stop() {
    live = 0;
}

void TFBParticles::update(float in_t) {
    dt = in_t - t;
    t = in_t;

    /*
    if (dt > 0) {
        vec3 vel = (Pos1 - Pos0) / dt;
        setVel(vel);
        // printf("vel: %f %f %f\n", vel.x, vel.y, vel.z);
    }
    */
}


void TFBParticles::render() {

    // glDisable(GL_DEPTH_TEST);
    // glBlendFunc(GL_ONE, GL_ONE);

    use();
    
    // update pass
    setUniform("update", 1);
    // setUniform("Time", t);
    setUniform("H", dt);
    setUniform("live", live);
    
    glEnable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
    glBeginTransformFeedback(GL_POINTS);
      glBindVertexArray(particleArray[1-drawBuf]);
      // TODO - could also be drawn without size parameter using another func
      // but needs initial particle
         
      glDrawArrays(GL_POINTS, 0, nParticles);
    glEndTransformFeedback();
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    // debug
    GLuint ptsWritten;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &ptsWritten);
    // printf("Written: %d\n", ptsWritten);
    glDisable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    // render
    setUniform("update", 0);
    glBindVertexArray(particleArray[1-drawBuf]);
    glDrawArrays(GL_POINTS, 0, nParticles);

    // Swap buffers
    drawBuf = 1 - drawBuf;

    // glEnable(GL_DEPTH_TEST);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TFBParticles::drawParticles() {
    glUseProgram(m_drawShader);

	safe_glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);

    safe_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, nParticles);

	safe_glDisableVertexAttribArray(0);
}


void TFBParticles::setTexture(GLuint textureName) {
    GLint location = safe_glGetUniformLocation(m_shaderProg, "uTexUnit");
    safe_glUniform1i(location, textureName);
}

void TFBParticles::setUniform(const char *name, vec3 v) {
    GLint location = safe_glGetUniformLocation(m_shaderProg, name);
    safe_glUniform3f(location, v);
}


void TFBParticles::setUniform(const char *name, const GLint val) {
    GLint location = safe_glGetUniformLocation(m_shaderProg, name);
    safe_glUniform1i(location, val);
}

void TFBParticles::setUniform(const char *name, const float val) {
    GLint location = safe_glGetUniformLocation(m_shaderProg, name);
    safe_glUniform1f(location, val);
}


void TFBParticles::setUniform(const char *name, const mat4 mat) {
    GLint location = safe_glGetUniformLocation(m_shaderProg, name);
    safe_glUniformMatrix4fv(location, value_ptr(mat));
}

void TFBParticles::setModelMatrix(mat4 mat) {
    GLint prog = m_shaderProg;
    glUseProgram(prog);
    GLint location = safe_glGetUniformLocation(prog,"uModelMatrix");
    safe_glUniformMatrix4fv(location, value_ptr(mat));
}

void TFBParticles::setViewMatrix(mat4 mat) {
    GLint prog = m_shaderProg;
    glUseProgram(prog);
    GLint location = safe_glGetUniformLocation(prog,"uViewMatrix");
    safe_glUniformMatrix4fv(location, value_ptr(mat));
}

void TFBParticles::setProjMatrix(mat4 mat) {
    GLint prog = m_shaderProg;
    glUseProgram(prog);
    GLint location = safe_glGetUniformLocation(prog,"uProjMatrix");
    safe_glUniformMatrix4fv(location, value_ptr(mat));
}


bool TFBParticles::installTFShader() {

    char *vShaderFName = (char *) "tfbparticles_vert.glsl";
    char *fShaderFName = (char *) "tfbparticles_frag.glsl";

    const GLchar *vShaderStr = textFileRead(vShaderFName);
    const GLchar *fShaderStr = textFileRead(fShaderFName);
    

    GLuint VS; //handles to shader object
    GLuint FS; //handles to frag shader object
    GLint vCompiled, fCompiled, linked; //status of shader

    VS = glCreateShader(GL_VERTEX_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);

    //load the source
    glShaderSource(VS, 1, &vShaderStr, NULL);
    glShaderSource(FS, 1, &fShaderStr, NULL);

    //compile vertex shader and print log
    glCompileShader(VS);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetShaderiv(VS, GL_COMPILE_STATUS, &vCompiled);
    cout << "Vertex ";
    printShaderInfoLog(VS);

    if (!vCompiled) {
        printf("Error compiling vertex shader '%s'\n", vShaderFName);
    }

    //compile frag shader and print log
    glCompileShader(FS);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetShaderiv(FS, GL_COMPILE_STATUS, &fCompiled);
    cout << "Frag ";
    printShaderInfoLog(FS);

    if (!vCompiled) {
        printf("Error compiling fragment shader '%s'\n", fShaderFName);
    }

    if (!vCompiled || !fCompiled) {
        return false;
    }

    //create a program object and attach the compiled shader
    m_shaderProg = glCreateProgram();
    glAttachShader(m_shaderProg, VS);
    glAttachShader(m_shaderProg, FS);

    // specify tfb vars - should be done before linking
    const char * names[] = {"Position", "Velocity", "StartTime"};
    glTransformFeedbackVaryings(m_shaderProg, 3, names, GL_SEPARATE_ATTRIBS);

    glLinkProgram(m_shaderProg);
    /* check shader status requires helper functions */
    printOpenGLError();
    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &linked);
    printProgramInfoLog(m_shaderProg);

    printf("sucessfully installed shaders '%s' and '%s'\n=================\n",
            vShaderFName, fShaderFName);
    return true;
}
