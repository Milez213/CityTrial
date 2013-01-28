vec3 v(0.0f);
float velocity, theta, phi;
GLfloat *data = new GLfloat[nParticles * 3];
for( GLuint i = 0; i<nParticles; i++ ) {
    // Pick the direction of the velocity
    theta = glm::mix(0.0f, (float)PI / 6.0f, randFloat());
    phi = glm::mix(0.0f, (float)TWOPI, randFloat());
    v.x = sinf(theta) * cosf(phi);
    v.y = cosf(theta);
    v.z = sinf(theta) * sinf(phi);
    // Scale to set the magnitude of the velocity (speed)
    velocity = glm::mix(1.25f,1.5f,randFloat());
    v = v * velocity;
    data[3*i] = v.x;
    data[3*i+1] = v.y;
    data[3*i+2] = v.z;
}
glBindBuffer(GL_ARRAY_BUFFER,initVel);

glBufferSubData(GL_ARRAY_BUFFER, 0,
        nParticles * 3 * sizeof(float), data);
