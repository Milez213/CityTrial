#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;

out vec3 Position;   // To transform feedback
out vec3 Velocity;   // To transform feedback
out float StartTime; // To transform feedback

out float Transp;    // To fragment shader
out vec3 color;

uniform float Time;  // Simulation time
uniform float H;     // Elapsed time between frames
uniform vec3 Accel;  // Particle acceleration
uniform float ParticleLifetime;  // Particle lifespan

uniform vec3 Pos0;
//uniform vec3 Vel0;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform int update;
uniform int live;


void Update() {
    // Update position & velocity for next frame
    Position = VertexPosition;
    Velocity = VertexVelocity;
    StartTime = VertexStartTime;
    
    float age = StartTime;

    if((live == 1) && age > ParticleLifetime) {
        // The particle is past it's lifetime, recycle.
        Position = Pos0; // vec3(0.0);
        Velocity = VertexInitialVelocity;
        StartTime = 0;
    } else {
        // The particle is alive, update.
        Position += Velocity * H;
        // Position.x += H * sin(10 * (Time + gl_VertexID));
        // Position.y += H * sin(10 * (Time) * noise1(Time));
        Velocity += Accel * H;
        StartTime += H;
    }

    if (Position.y < -1) {
        Velocity.y *= -0.8;
    }
}

void Render() {
    float age = VertexStartTime;
    Transp = 1.0 - age / ParticleLifetime;

    color = vec3(1, Transp, 0.0);
    /*
    if (gl_VertexID % 2 == 0) {
        color = vec3(1, Transp, 0.0);
    } else {
        color = vec3(1-Transp, Transp, 1);
    }
    */

    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(VertexPosition, 1.0);

    if (gl_Position.z > 0.2)
        gl_PointSize = 20/(gl_Position.z);
    else {
        gl_PointSize = 20;
    }

}

void main()
{
  if (update == 1) {
    Update();
  } else {
    Render();
  }
}
