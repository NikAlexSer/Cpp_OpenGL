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

#include "SimpleCamera.h"


/// Каркас нового базового приложения:
class App{
	public:
		/// Выполняем инициализацию:
		void setup(int w, int h, string captionWindow, bool useAntiAliasing);
		/// Выполняем рендеринг одного кадра:
		void draw();
		/// Выполняем обновление всех C++ переменных - всей логики приложения:
		void update();
		/// Выход из приложения:
		void exit();
		/// Инициализируем геометрию и все что с этим может быть связано, все что потом так или иначе будем рисовать на экране:
		void setupScene();


		/// Базовые обработчики событий:
		/// Данные функции вызываются когда происходят события нажатия на клавиатуру, события с мышью и возмыжные неполадки:
		static void error_callback(int error, const char* description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow* window, int a, int b, int c);

		static App& getInstance(){
			static App app;
			return app;
		}

		/// Получить окно:
		GLFWwindow* getWindow(){
			return window;
		}

		
		vec3  modelPos;
		vec3  modelScale;
		vec3  angles;

		

	private:
		/// Ширина и высота окна GL:
		int width, height;
		/// Указатель на используемое окно:
		GLFWwindow* window;

		/// Загружаем шейдер:
		GlslShader shader;

		/// Глобальная матрица:
		mat4 globalMVP;

		/// Матрица модели:
		mat4 model;

		/// Идентификатор вершинного буфера:
		GLuint vertexBuffer;

		/// Идентификатор цветового буфера:
		GLuint colorBuffer;

		SimpleCamera camera;
};