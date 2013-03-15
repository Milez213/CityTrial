


varying vec2 texpos;
uniform sampler2D tex;
uniform vec3 color;

void main(void) {
  gl_FragColor = vec4(color[0], color[1], color[2], texture2D(tex, texpos).a);
}



