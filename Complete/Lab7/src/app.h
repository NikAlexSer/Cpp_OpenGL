#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "GlslShader.h"
#include "SimpleCamera.h"
#include "SimplePointLight.h"
#include "Skybox.h"
#include "SimpleLoaderCollada3D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class App {
	public:
		void setup(int w, int h, string captionWindow, bool useAntiAliasing);
		void draw();
		void update();
		void exit();
		void setupScene();
		void drawTextureCube();
		void createTextureCube();

		static void error_callback(int error, const char* description);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow* window, int a, int b, int c);

		static App& getInstance() {
			static App app;
			return app;
		}

		vec3 modelPos;
		vec3 modelScale;
		vec3 axis;
		vec3 angles;

		GLuint texture1;
		GLuint texture2;

		GLFWwindow* getWindow() {
			return window;
		}

		GLuint cubeVAO;

	private:
		int width, height;
		GLFWwindow* window;
		GlslShader shader;
		GlslShader shader3Dmodel;
		SimpleCamera camera;
		loaderImage loader;
		SkyBox skyBox;
		SimpleLoaderCollada3D house;
		SimpleLoaderCollada3D ground;
		SimpleLoaderCollada3D teapot;
		SimplePointLight lightPoint;
		mat4 globalMVP;
		mat4 model;
		GLuint vertexBuffer;
		GLuint colorBuffer;
		GLuint uvBuffer;
		float mixFactor;
};