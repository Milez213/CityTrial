//#version 330

attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main() {
    vec4 vPosition;

    vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
    gl_Position = uProjMatrix * uViewMatrix * vPosition;
}
