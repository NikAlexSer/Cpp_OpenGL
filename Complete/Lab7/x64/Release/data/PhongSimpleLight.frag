#version 330 core

in vec2 UV;
out vec3 color;
in vec3 fragmentColor;

uniform sampler2D diffuseTexture;
uniform vec3 lightColor;

in vec3 vertexPosCameraSpace;
in vec3 normalVector;
in vec3 lightDirection;  


void main(){
	vec3 objColor = texture(diffuseTexture, vec2(UV.x,1.0 - UV.y)).rgb;
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;  
	vec3	l		= normalize(lightDirection);
	vec3	n		= normalize(normalVector);  

	float	diff	= clamp(dot(n, l), 0, 1);
	vec3 diffuse	= diff * lightColor;

    float specularStrength	= 1.0f;
    vec3  viewDir			= normalize(-vertexPosCameraSpace); 
    vec3	reflectDir  = reflect(-l, n);  
	float	spec		= pow(clamp(dot(viewDir, reflectDir), 0,1), 32);
    vec3	specular	= specularStrength * spec * lightColor; 
	vec3 result = (ambient + diffuse + specular) * objColor;
	color = result;
}


