attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
attribute vec2 aNrmCoord;

uniform vec4 uDiffuse;
uniform vec4 uSpecular;
uniform vec4 uShaderFlags;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

varying vec4 vPosition;
varying vec4 vNormal;
varying vec2 vTexCoord;

void main() {
  
  /* First model transforms */
  vPosition = uModelMatrix* vec4(aPosition.x, aPosition.y, aPosition.z, 1);
  vNormal = uNormalMatrix* vec4(aNormal.x, aNormal.y, aNormal.z, 1);
  vPosition = uViewMatrix* vPosition;
  if (uShaderFlags.w == 1.0)
  		gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1);
  else
  		gl_Position = uProjMatrix*vPosition;
  //gl_Position = vec4(0, 0, 0, 1);

  vPosition = vec4(aPosition.x, aPosition.y, aPosition.z, 1);
  vTexCoord = aTexCoord;
  //gl_FrontColor = vec4(1.0, 1.0, 0.0, 1.0);
}
