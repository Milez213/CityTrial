#version 330

layout (location = 0) in vec3 VertexPosition;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main() {
    vec4 posView = uViewMatrix * uModelMatrix * vec4(VertexPosition, 1.0);
    gl_Position = uProjMatrix * posView;
}

