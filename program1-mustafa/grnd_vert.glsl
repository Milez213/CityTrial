#version 330

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aColor;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main() {
    vec4 vPosition;
    vec4 tNormal;

    /* First model transforms */
    vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
    gl_Position = uProjMatrix * uViewMatrix * vPosition;
}
