// #version 320

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTextCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uiModelMatrix;

varying vec3 vNormal;
varying vec4 vPosition;
varying vec2 vTextCoord;

void main() {
    vec4 tNormal;

    /* First model transforms */
    vPosition = uModelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1);
    gl_Position = uProjMatrix * uViewMatrix * vPosition;

    // tNormal = normalize(transpose(inverse(uModelMatrix)) * vec4(aNormal, 0));
    tNormal = normalize(uiModelMatrix * vec4(aNormal, 0));

    // pass
    vNormal = tNormal.xyz;
    
    vTextCoord = aTextCoord;
}
