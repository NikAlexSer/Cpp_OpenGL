#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform vec3 lightColor;

void main(){	
	gl_Position = MVP * modelMatrix * vec4(vertexPosition_modelspace,1);	
	fragmentColor = vertexColor;
 }
