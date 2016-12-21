#version 330 core

in vec2 UV;
out vec3 color;
in vec3 fragmentColor;
uniform sampler2D diffuseTexture;

void main(){
	color = texture(diffuseTexture, vec2(UV.x, 1.0 - UV.y)).rgb;
}