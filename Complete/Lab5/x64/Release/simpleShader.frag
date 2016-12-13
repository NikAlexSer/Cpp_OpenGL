#version 330 core
in vec2 UV;
out vec3 color;
uniform sampler2D shaderTexture1;

void main(){
    color = texture(shaderTexture1,UV).rgb;
}
