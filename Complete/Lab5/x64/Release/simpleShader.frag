#version 330 core
in vec2 UV;
out vec3 color;

in vec3 fragmentColor;
uniform sampler2D shaderTexture1;
uniform sampler2D shaderTexture2;
uniform float mixFactor;

void main(){
    color = mix(texture(shaderTexture1, UV), texture(shaderTexture2, UV), mixFactor).rgb;
}
