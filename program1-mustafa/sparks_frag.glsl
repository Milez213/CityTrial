#version 330

in float Transp;
in vec3 color;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    
    FragColor = vec4(color, Transp);

    // try to draw round dots
    /*
    if (length( (2*gl_PointCoord - vec2(1,1))) > 1) {
        discard;
        //FragColor = vec4(color, 0.0);
    } else {
        FragColor = vec4(color, 0.9 * Transp);
    }
    */

}
