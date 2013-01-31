uniform sampler2D uTexUnit;

uniform vec4 uDiffuse;
uniform vec4 uSpecular;
uniform vec4 uShaderFlags;

uniform vec4 uSun;
uniform vec3 uCamera;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

varying vec4 vPosition;
varying vec4 vNormal;
varying vec2 vTexCoord;

void main(void) 
{
	vec4 dColor;
	if (uShaderFlags.x == 1.0) {
		dColor = texture2D(uTexUnit, vTexCoord);
	} else {
		dColor = vec4(uDiffuse.r, uDiffuse.g, uDiffuse.b, uDiffuse.w);
	}
	vec3 vPos = vec3(vPosition.x, vPosition.y, vPosition.z);
	vec3 vNor = normalize(vec3(vNormal.x, vNormal.y, vNormal.z));
	vec3 lPos = vec3(uSun.x, uSun.y, uSun.z);
	vec3 lDir = normalize(lPos - vPos);

	float dirNor = dot(lDir, vNor);
	if (dirNor < 0.0)
		dirNor = 0.0;
	
	//Flip you some normals starText.obj
	//dirNor = 1.0 - dirNor;
	
	vec3 calcDiffuse;
	calcDiffuse.r = dColor.r * dirNor * uSun.w;
	calcDiffuse.g = dColor.g * dirNor * uSun.w;
	calcDiffuse.b = dColor.b * dirNor * uSun.w;
	
	vec3 cDir = normalize(uCamera - vPos);
	lDir = normalize(lPos - vPos);
	vec3 dif = normalize(lDir + cDir);
	vec3 calcSpecular;
	
	float difNor = dot(dif, vNor);
	if (difNor < 0.0)
		difNor = 0.0;
		
	calcSpecular.r = uSpecular.r * pow(difNor, 10.0) * uSun.w / 2.0;
	calcSpecular.g = uSpecular.g * pow(difNor, 10.0) * uSun.w / 2.0;
	calcSpecular.b = uSpecular.b * pow(difNor, 10.0) * uSun.w / 2.0;
	
	vec3 colFin;
	colFin.r = calcDiffuse.r + calcSpecular.r + dColor.r * 0.2;
	colFin.g = calcDiffuse.g + calcSpecular.g + dColor.g * 0.2;
	colFin.b = calcDiffuse.b + calcSpecular.b + dColor.b * 0.2;
	
	if (uShaderFlags.w == 1.0)
		gl_FragColor = vec4(dColor.r, dColor.g, dColor.b, 1.0);
	else
		gl_FragColor = vec4(colFin.r, colFin.g, colFin.b, 1.0);
}