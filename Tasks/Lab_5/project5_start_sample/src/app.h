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


/// ������ ������ �������� ����������:
class App{
	public:
		/// ��������� �������������:
		void setup(int w, int h, string captionWindow, bool useAntiAliasing);
		/// ��������� ��������� ������ �����:
		void draw();
		/// ��������� ���������� ���� C++ ���������� - ���� ������ ����������:
		void update();
		/// ����� �� ����������:
		void exit();
		/// �������������� ��������� � ��� ��� � ���� ����� ���� �������, ��� ��� ����� ��� ��� ����� ����� �������� �� ������:
		void setupScene();


		/// ������� ����������� �������:
		/// ������ ������� ���������� ����� ���������� ������� ������� �� ����������, ������� � ����� � ��������� ���������:
		static void error_callback(int error, const char* description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow* window, int a, int b, int c);

		static App& getInstance(){
			static App app;
			return app;
		}

		/// �������� ����:
		GLFWwindow* getWindow(){
			return window;
		}

		
		vec3  modelPos;
		vec3  modelScale;
		vec3  angles;

		

	private:
		/// ������ � ������ ���� GL:
		int width, height;
		/// ��������� �� ������������ ����:
		GLFWwindow* window;

		/// ��������� ������:
		GlslShader shader;

		/// ���������� �������:
		mat4 globalMVP;

		/// ������� ������:
		mat4 model;

		/// ������������� ���������� ������:
		GLuint vertexBuffer;

		/// ������������� ��������� ������:
		GLuint colorBuffer;

		SimpleCamera camera;
};