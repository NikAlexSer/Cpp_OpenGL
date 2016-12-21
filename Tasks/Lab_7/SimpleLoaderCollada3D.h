#pragma once

#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GlslShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <sstream>
#include <stddef.h>  
using namespace std;
using namespace glm;

#include "loadTexture.h"
#include "tinyxml\tinyxml.h"

struct Vertex {
    /// Координата вершины:
    vec3 position;
    /// Нормаль:
    vec3 normal;
    // Текустурная координата:
    vec2 texCoords;
};


/// Класс загрузки 3D модели в формате collada
/// Пока реализуем лишь загрузку самой геометрии(mesh) из треугольников
/// Будем читать из файла координаты вершин, координаты нормалей вершин и текстурные координаты вершин
/// А также индексы треугольников которые будут связывать массивы координат вершин, текстурных координат, и координат нормалей:
class SimpleLoaderCollada3D {
	public:
		/// Указываем папку с 3D моделью:
		bool setup(string folder);
		/// Обновляем состояние модели, в будущем её анимации:
		void update(mat4 MVP);
		void draw();

		void setScale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotate(float angle, vec3 axis);

	private:	
		mat4 globalMVP;
		mat4 modelMatrix;

		/// Вспомогательные функции для парсинга строки:
		void split(const std::string &s, char delim, std::vector<string> &elems);
		std::vector<std::string> split(const std::string &s, char delim);

		/// Получить массив 3D/2D координат из тега содержащий числа разделенные пробелами:
		template<typename T> 
		void getPointsArrayFromStringTag(TiXmlElement* tag, vector <T>& pointsOut);


		/// Описание сетки:
		vector <glm::vec3> coords;
		vector <glm::vec3> normals;
		vector <glm::vec2> uvCoords;
		/// Конечный массив собранных в правильном порядке вершин:
		vector <Vertex> outVertices;

		/// ID вершинного буфера:
		GLuint VAO, VBO;
		/// Диффузивная текстура:
		GLuint diffuseTexture;
		/// Загружаем шейдер:
		GlslShader shader;
};