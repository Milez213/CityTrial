uniform sampler2D uTexUnit;

uniform mat4 uModelMatrix;

varying vec4 vPosition;
varying vec2 vTexture;

void main(void) {
   vec4 color;
   color = texture2D(uTexUnit, vTexture);

   /*
   if (color.r < 0.9 || color.g < 0.9 || color.b < 0.9)
      gl_FragColor = vec4(color.rgb, 1.0);
   
   else
      gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);

   */
   
   // use alpha value in texture
   gl_FragColor = color;
}



