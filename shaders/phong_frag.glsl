#version 120

struct Material {
  vec3 aColor;
  vec3 dColor;
  vec3 sColor;
  float shine;
};

// texture
uniform sampler2D uTexUnit;

// light info
uniform vec3 uLightPos;
uniform vec3 uLColor;
uniform Material uMat;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uCamPos;

uniform int uShowNormals;

varying vec4 vPosition;
varying vec3 vNormal;
varying vec2 vTextCoord;

uniform int isLit;

void main() {


if (isLit == 1){
   gl_FragColor = vec4(texture2D(uTexUnit, vTextCoord).xyz,1.0);
}

else {
    vec3 L;
    vec3 diffuse;
    vec3 spec;
    vec3 N;
    vec3 V;
    vec4 finalColor;
	float NL;

    vec3 dColor;
    if (uMat.dColor.r != -1.0)
      dColor = uMat.dColor;
    else
      dColor = texture2D(uTexUnit, vTextCoord).xyz;
    
    // interpolated normal
    N = normalize(vNormal);

    // from
    // http://www.ozone3d.net/tutorials/glsl_fog/p04.php
    const float LOG2 = 1.442695f;
    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fogDensity = 0.01; // how foggy it is
    float fogFactor = exp2(- z * z * fogDensity * fogDensity * LOG2);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    if (uShowNormals == 0) {
        L = normalize(uLightPos - vPosition.xyz);

		NL = max(dot(N, L), 0.0);

        diffuse = dColor * NL * uLColor;

        V = normalize(uCamPos - vPosition.xyz);

		if (NL > 0.0) {
			spec = uMat.sColor *
               pow(max(dot(V, -L + 2.0 * NL * N), 0.0), 20.0) *
               uLColor;
		} else {
			// facing backwards
			spec = vec3(0.0);
		}

        finalColor = vec4(clamp(spec + diffuse + dColor * 0.2, 0.0, 1.0), 1.0);

        gl_FragColor = mix(vec4(0.8, 0.8, 1.0, 1.0), finalColor, fogFactor);
        // gl_FragColor = mix(vec4(1.0), vec4(0.0, 1, 0.0), fogFactor);

    } else {
        gl_FragColor = vec4(N, 1.0);
    }
}
}
