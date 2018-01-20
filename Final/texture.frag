#version 150
// texture fragment shader for table
//
// Contributor:  Dhaval Chauhan (dmc8686)

uniform float ambRefCoeff;
uniform float diffRefCoeff;
uniform float specRefCoeff;
uniform float specExponent;

uniform vec4 lightSourceColor;
uniform vec4 sceneAmbLightColor;

uniform sampler2D clothTexture;

in vec3 normal;
in vec3 light;
in vec3 viewing;
in vec2 texCoordinates;

out vec4 finalColor;

void main()
{
	// Vector to hold texture specific data.
	vec4 tex;
	
	// N, V, L, R vectors
	vec3 vectorN;
	vec3 vectorV;
	vec3 vectorL;
	vec3 vectorR;
	
	// If smiley face is active.
    if(gl_FrontFacing)
    {
		// Convert texture in vector
		tex = texture(clothTexture, texCoordinates);
		
		// Compute vector N
		vectorN = normalize( normal );
	}
	else
	{
		// Convert texture in vector
		tex = texture(clothTexture, texCoordinates);
		
		// Compute vector N
		vectorN = normalize( -normal );
	}
	
	// Compute other vectors
	vectorV = normalize( viewing );
	vectorL = normalize( light - viewing );
	vectorR = normalize( reflect( vectorL, vectorN) );
	
	//Apply Ambient, Diffuse and Specular lighting to quad.
	vec4 amb = tex * ambRefCoeff * sceneAmbLightColor;
	vec4 dif = tex * diffRefCoeff * max(0.0, dot( vectorN, vectorL )) * lightSourceColor;
	vec4 spec = tex * specRefCoeff * pow( max(0.0, dot( vectorV, vectorR )), specExponent ) * lightSourceColor;
	
	//Result
    finalColor = amb + dif + spec;
}
