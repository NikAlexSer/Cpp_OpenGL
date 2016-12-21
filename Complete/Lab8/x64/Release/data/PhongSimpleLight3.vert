#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 modelMatrix;
uniform vec3 lightPos; 
uniform vec3 viewPos;

out VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vsOut;

void main(){	
	vec4 vertex			= vec4(vertexPos,1.0f);
	gl_Position			= P * V * M * modelMatrix * vertex;	
	vsOut.texCoords		= vertexUV;
	vsOut.fragPos		= vec3( M * modelMatrix * vertex);	
	
	mat3 normalMatrix	= transpose(inverse(mat3(M * modelMatrix)));

	vec3 T				= normalize(normalMatrix * tangent);
    vec3 B				= normalize(normalMatrix * biTangent);
    vec3 N				= normalize(normalMatrix * normal);    
	mat3 TBN			= transpose(mat3(T, B, N));
	
	vsOut.tangentLightPos = TBN * lightPos;
    vsOut.tangentViewPos  = TBN * normalize(viewPos);
    vsOut.tangentFragPos  = TBN * vsOut.fragPos;
 }
  

