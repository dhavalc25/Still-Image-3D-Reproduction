#version 150

// Phong fragment shader for objects with no textures
//
// Contributor:  Dhaval Chauhan (dmc8686)

uniform vec4 ambMatColor;
uniform vec4 diffMatColor;
uniform vec4 specMatColor;

uniform float ambRefCoeff;
uniform float diffRefCoeff;
uniform float specRefCoeff;
uniform float specExponent;

uniform vec4 lightSourceColor;
uniform vec4 lightSourcePosition;
uniform vec4 sceneAmbLightColor;

// INCOMING DATA
in vec3 normal;
in vec3 light;
in vec3 viewing;

// OUTGOING DATA
out vec4 finalColor;

void main()
{
	//Compute vectors N, L, V, and R.
	vec3 vectorN = normalize( normal );
	vec3 vectorV = normalize( viewing );
	vec3 vectorL = normalize( light - viewing);
	vec3 vectorR = normalize( reflect( vectorL, vectorN));		//reflect
	
	//Apply Ambient, Diffuse and Specular lighting to teapot.
	vec4 amb = ambMatColor * ambRefCoeff  * sceneAmbLightColor;
	vec4 dif = diffMatColor * diffRefCoeff* max(0.0, dot( vectorN, vectorL )) * lightSourceColor;
	vec4 spec = specMatColor * specRefCoeff * pow( max(0.0, dot( vectorV, vectorR )), specExponent ) * lightSourceColor;
	
	//Result
    finalColor = amb + dif + spec;
}
