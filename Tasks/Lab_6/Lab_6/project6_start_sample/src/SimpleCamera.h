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

using namespace std;
using namespace glm;


/// Заголовок класса виртуальной камеры:
class SimpleCamera{
	public:
		void	setup(GLFWwindow* window, vec3 pos, float hAngle, float vAngle, float fov, float speed, float mouseSpeed);
		void	update();

		/// Для передачи этой матрицы в шейдер:
		mat4	getCameraMatrix(){
			return resultMVP;
		}
	private:
		/// Глобальная результирующая матрица:
		mat4 resultMVP;
		/// 3 стандартных матрицы OpenGL:
		mat4 projectionMatrix;
		mat4 viewMatrix;
		mat4 modelMatrix;

		/// Позиция камеры:
		vec3 pos;
		/// Горизонтальный угол:
		float hAngle;
		/// Вертикальный угол:
		float vAngle;
		/// Начальный угол обзора:
		float initialFov;
		/// Скорость движения камеры:
		float speed;
		/// Скорость реакции на движения мыши:
		float mouseSpeed;

		/// Указатель на используемое окно:
		GLFWwindow* window;
		/// Размеры окна:
		int widthWindow,heightWindow;

		float lastTime;
};