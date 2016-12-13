#version 330 core
/// Передача вершин(число 0)
layout(location = 0) in vec3 vertexPosition_modelspace;

void main(){	
	/// vertexPosition_modelspace - координаты вершин получаемых из VAO:
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;
 }