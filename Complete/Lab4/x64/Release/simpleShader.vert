#version 330 core
/// �������� ������(����� 0)
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;

uniform mat4 MVP;

void main(){	
	/// vertexPosition_modelspace - ���������� ������ ���������� �� VAO:	
	/// ���������� �������, c w = 1.0:
	vec4 v = vec4(vertexPosition_modelspace,1);
	gl_Position = MVP * v;	

	/// ���� ������ ������� ����� ��������������
	/// ����� ���������� ���� ������� ���������
	fragmentColor = vertexColor;

 }