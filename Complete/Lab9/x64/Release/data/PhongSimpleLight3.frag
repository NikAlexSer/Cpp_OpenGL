#version 330 core
uniform sampler2D	diffuseTexture;
uniform sampler2D	specularTexture;
uniform sampler2D	normalTexture;
uniform bool		useSpecularMap;
uniform vec3		lightColor;
uniform float		specularStrength;
out vec3 color;

in vec2 UV;
in vec3 vertexPosCameraSpace;
in vec3 normalVector;
in vec3 lightDirection;  

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fsIn;

void main(){
	vec3 normal		= texture(normalTexture, vec2(fsIn.texCoords.x,1.0 - fsIn.texCoords.y)).rgb;
	normal			= normalize(normal * 2.0 - 1.0) ;
	vec3 objColor = texture(diffuseTexture, vec2(fsIn.texCoords.x,1.0 - fsIn.texCoords.y)).rgb;	
	float ambientStrength	= 0.1f;
    vec3  ambient			= ambientStrength * lightColor;    
    vec3 lightDir = normalize(fsIn.tangentLightPos - fsIn.tangentFragPos);
    float diff = clamp(dot(normal, lightDir), 0, 1);
    vec3 diffuse = diff * lightColor;

	vec3 specObjColor	= vec3(1.0);
	if (useSpecularMap) specObjColor = texture(specularTexture, vec2(fsIn.texCoords.x,1.0 - fsIn.texCoords.y)).rgb;


    vec3	viewDir		= normalize(fsIn.tangentViewPos - fsIn.tangentFragPos);
	vec3	reflectDir	= reflect(-lightDir, normal);
	float	spec		= pow(clamp(dot(viewDir, reflectDir), 0,1), 32);
    vec3	specular	= vec3(specularStrength) * spec * specObjColor * lightColor;
	color	= (ambient + diffuse + specular) * objColor;
}
