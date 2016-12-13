#version 330 core
/// Передача вершин(число 0)
layout(location = 0) in vec3 vertexPosition_modelspace;
/// Передача цветов вершин(число 1)
layout(location = 1) in vec2 vertexUV;

uniform mat4 MVP;

// Исходящие данные: будут интерполированы для каждого фрагмента
out vec2 UV;

void main(){	
	/// vertexPosition_modelspace - координаты вершин получаемых из VAO:	
	/// Координата вершины, c w = 1.0:
	vec4 v = vec4(vertexPosition_modelspace,1);
	gl_Position = MVP * v;	
	/// координата UV данной вершины. Тут никаких особых преобразований не нужно делать:
	UV = vertexUV;
 }

