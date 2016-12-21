#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform float sizeSkyBox;

void main(){
	gl_Position = MVP * modelMatrix * vec4(position*sizeSkyBox,1.0);
    TexCoords = position;
}  