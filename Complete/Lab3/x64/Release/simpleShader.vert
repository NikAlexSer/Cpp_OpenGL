#version 330 core
/// �������� ������(����� 0)
layout(location = 0) in vec3 vertexPosition_modelspace;

void main(){	
	/// vertexPosition_modelspace - ���������� ������ ���������� �� VAO:
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;
 }