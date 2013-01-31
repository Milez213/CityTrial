//#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

out vec3 vNormal;
out vec4 vPosition;

void main() {
    vec4 tNormal;

    /* First model transforms */
    vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    tNormal = normalize(transpose(inverse(uModelMatrix)) * vec4(aNormal, 0));

    // pass
    vNormal = tNormal.xyz;
}
