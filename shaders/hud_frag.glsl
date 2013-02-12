uniform sampler2D uTexUnit;

uniform mat4 uModelMatrix;

varying vec4 vPosition;
varying vec2 vTexture;

void main(void) {
   vec4 color;
   color = texture2D(uTexUnit, vTexture);
   
   gl_FragColor = vec4(color.gbr, 1.0);
}



