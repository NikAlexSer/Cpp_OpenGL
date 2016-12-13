#version 330 core
/// �������� ������(����� 0)
layout(location = 0) in vec3 vertexPosition_modelspace;
/// �������� ������ ������(����� 1)
layout(location = 1) in vec2 vertexUV;

uniform mat4 MVP;

// ��������� ������: ����� ��������������� ��� ������� ���������
out vec2 UV;

void main(){	
	/// vertexPosition_modelspace - ���������� ������ ���������� �� VAO:	
	/// ���������� �������, c w = 1.0:
	vec4 v = vec4(vertexPosition_modelspace,1);
	gl_Position = MVP * v;	
	/// ���������� UV ������ �������. ��� ������� ������ �������������� �� ����� ������:
	UV = vertexUV;
 }

