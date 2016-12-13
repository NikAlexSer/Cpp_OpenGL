#version 330 core
/// ѕередача вершин(число 0)
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 MVP;

void main(){	
	/// vertexPosition_modelspace - координаты вершин получаемых из VAO:	
	///  оордината вершины, c w = 1.0:
	vec4 v = vec4(vertexPosition_modelspace,1);
	gl_Position = v * MVP;	
 }