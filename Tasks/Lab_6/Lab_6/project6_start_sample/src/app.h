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

		/// �������� �������� �� bmp �����:
		GLuint loadBmp(string fileName);
		/// �������� ������ �������� � mip-map ��������:
		GLuint loadDDS(string fileName);


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

		GLuint texture1;
		GLuint texture2;

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
		//GLuint colorBuffer;

		/// ���������� ����������:
		GLuint uvBuffer;

		SimpleCamera camera;


		float mixFactor;
};