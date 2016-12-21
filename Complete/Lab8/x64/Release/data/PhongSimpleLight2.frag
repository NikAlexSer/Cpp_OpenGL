#version 330 core

in vec2 UV;
out vec3 color;
in vec3 fragmentColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform vec3 lightColor;
uniform bool useSpecularMap;
uniform float specularStrength;

in vec3 vertexPosCameraSpace;
in vec3 normalVector;
in vec3 lightDirection;  

void main(){
	vec3 objColor = texture(diffuseTexture, vec2(UV.x,1.0 - UV.y)).rgb;
	vec3 specObjColor = vec3(1.0);
	if (useSpecularMap) specObjColor = texture(specularTexture, vec2(UV.x, 1.0-UV.y)).rgb;

	float lightPower = 1.0f;
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;  
	vec3	l		= normalize(lightDirection);
	vec3	n		= normalize(normalVector);  

	float distance = length(lightDirection);
	float	diff	= clamp(dot(n, l), 0, 1);
	vec3 diffuse	= diff * lightColor * lightPower / (distance*distance);
	
    vec3  viewDir			= normalize(-vertexPosCameraSpace); 
    vec3	reflectDir  = reflect(-l, n);  
	float	spec		= pow(clamp(dot(viewDir, reflectDir), 0,1), 32);
    vec3	specular	= specularStrength * spec * specObjColor * lightColor; 
	vec3 result = (ambient + diffuse + specular) * objColor;
	color = result;
}


