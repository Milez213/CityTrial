uniform sampler2D uTexUnit;

uniform mat4 uModelMatrix;
uniform vec4 uColor;

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
   if (uColor.a == 0.0) 
      gl_FragColor = color;
   else
      gl_FragColor = vec4(uColor.r * color.r, uColor.g * color.g, 
      uColor.b * color.b, color.a);
}



