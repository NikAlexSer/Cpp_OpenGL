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

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

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

		/// Загрузка текстуры из bmp файла:
		GLuint loadBmp(string fileName);
		/// Загрузка сжатой текстуры с mip-map уровнями:
		GLuint loadDDS(string fileName);


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

		GLuint texture1;
		GLuint texture2;

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
		//GLuint colorBuffer;

		/// Текстурные координаты:
		GLuint uvBuffer;

		SimpleCamera camera;


		float mixFactor;
};