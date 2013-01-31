#version 330

//uniform sampler2D ParticleTex;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    // FragColor = vec4(1, 1, 1, 1); // texture(ParticleTex, gl_PointCoord);
    
    FragColor = vec4(1, 0, 0, 1.0);
}

