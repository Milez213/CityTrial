//#version 330

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

in vec4 vPosition;
in vec3 vNormal;

layout ( location = 0 ) out vec4 FragColor;

void main() {
    vec3 L;
    vec3 diffuse;
    vec3 spec;
    vec3 N;
    vec3 V;
    vec3 finalColor;
	float NL;

    vec3 dColor = texture2D(uTexUnit, 0.3*vPosition.xz).xyz;
    
    // interpolated normal
    N = normalize(vNormal);

    if (uShowNormals == 0) {
        L = normalize(uLightPos - vPosition.xyz);

		NL = max(dot(N, L), 0.0);

        diffuse = dColor * NL * uLColor;

        V = normalize(uCamPos - vPosition.xyz);

		if (NL > 0.0) {
			spec = uMat.sColor *
               pow(max(dot(V, -L + 2.0 * NL * N), 0.0), uMat.shine) *
               uLColor;
		} else {
			// facing backwards
			spec = vec3(0.0);
		}

        FragColor = vec4(clamp(spec + diffuse + uMat.aColor, 0.0, 1.0), 1.0);
    } else {
        FragColor = vec4(N, 1.0);
    }

	//gl_FragColor = uCamPos;
}
