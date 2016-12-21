#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertexUV;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;
uniform mat4 modelMatrix;

out vec2 UV;


void main(){	
	vec4 v = vec4(vertexPosition_modelspace,1);
	gl_Position = P * V * M * modelMatrix * v;	
	UV = vertexUV;
}