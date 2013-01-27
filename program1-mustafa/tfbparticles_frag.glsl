#version 330

uniform sampler2D uTexUnit;

in float Transp;
in vec3 color;

layout ( location = 0 ) out vec4 FragColor;

void main()
{


    FragColor = texture(uTexUnit, gl_PointCoord);

    // FragColor.a *= Transp;
    
    // FragColor = vec4(color, Transp);

    // try to draw round dots
    /*
    if (length( (2*gl_PointCoord - vec2(1,1))) > 1) {
        discard;
        //FragColor = vec4(color, 0.0);
    } else {
        FragColor = vec4(color, 0.9 * Transp);
    }
    */

    // FragColor.a *= 1 - length( (2*gl_PointCoord - vec2(1,1)));
}
