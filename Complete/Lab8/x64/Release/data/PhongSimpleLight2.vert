 #version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertexUV;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 modelMatrix;
uniform vec3 lightPos; 

out vec2 UV;
out vec3 vertexPosCameraSpace;
out vec3 normalVector;
out vec3 lightDirection;

void main(){	
	vec4 v = vec4(vertexPos,1);
	gl_Position = P * V * M * modelMatrix * v;	
	UV = vertexUV;
	vec4 vertPos4				= V * M * modelMatrix * v;
    vertexPosCameraSpace		= vec3(vertPos4) / vertPos4.w;
	normalVector				= mat3(transpose(inverse(V * M * modelMatrix ))) * normal;
	vec3 lightPosCameraSpace	= ( V *  vec4(lightPos,1)).xyz;
	lightDirection				= lightPosCameraSpace - vertexPosCameraSpace;
 }