attribute vec3 aPosition;
attribute vec2 aTexture;

uniform mat4 uModelMatrix;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;

varying vec4 vPosition;
varying vec2 vTexture;

void main(void) {
   
   vPosition = uModelMatrix* vec4(aPosition.xyz, 1);
   vPosition = uViewMatrix* vPosition;
   
   vTexture = aTexture;
   
   gl_Position = uProjMatrix * vPosition;
   
   //gl_FrontColor = vec4(1.0, 0.0, 1.0, 1.0);
}



