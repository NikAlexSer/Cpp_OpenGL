#pragma once

#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GlslShader.h"
#include "SimplePointLight.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
using namespace glm;

class SimplePointLight {
public:
	/// Выполняем инциализацию:
	void setup(GLFWwindow* window, float r, float g, float b);
	/// Рисуем позицию источника света в виде куба, передаем матрицу камеры:		
	void draw(mat4 MVP);

	/// Работаем с клавиатурой чтобы сделать более наглядным работу источник освещения:
	void update();

	/// Задаем позицию:
	void setPosition(float x, float y, float z) {
		vec3 p;
		p.x = x;
		p.y = y;
		p.z = z;
		setPosition(p);
	}

	void setPosition(vec3 pos) {
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, vec3(pos.x, pos.y, pos.z));
		modelMatrix = glm::scale(modelMatrix, vec3(0.1, 0.1, 0.1));
		this->pos = pos;
	}

	vec3 getLightPos() {
		return pos;
	}

	vec3 getColor() {
		return color;
	}

private:
	/// Модельно видовая матрица:
	mat4 modelMatrix;
	/// Положение света в пространстве:
	vec3	pos;
	/// Цвет света:
	vec3    color;
	/// Идентификатор Vertex Array Object для нашего куба:
	GLuint	cubeVAO;
	/// Идентификатор вершинного буфера:
	GLuint vertexBuffer;
	/// Идентификатор цветового буфера:
	GLuint colorBuffer;

	/// Загружаем шейдер:
	GlslShader shader;

	/// Последние время вызова update():
	float lastTime;
	/// Окно нашего приложения, для доступа к клавиатуре:
	GLFWwindow* window;
};
