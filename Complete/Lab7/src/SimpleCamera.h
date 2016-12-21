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

using namespace std;
using namespace glm;


class SimpleCamera {
	public:
		void setup(GLFWwindow* window, vec3 pos, float hAngle, float vAngle, float fov, float speed, float mouseSpeed, float farThreshold);
		void update();

		// Для передачи этой матрицы в шейдер
		mat4& getCameraMatrix() {
			return resultMVP;
		};

		mat4& getCameraProjectionMatrix() {
			return projectionMatrix;
		}
		mat4& getCameraViewMatrix() {
			return viewMatrix;
		}
		mat4& getCameraModelMatrix() {
			return modelMatrix;
		}

		vec3& getPosition() {
			return pos;
		}

	private:
		mat4 resultMVP;
		mat4 projectionMatrix;
		mat4 viewMatrix;
		mat4 modelMatrix;

		vec3 pos;
		float hAngle;
		float vAngle;
		float initialFov;
		float speed;
		float mouseSpeed;
		GLFWwindow* window;
		int widthWindow, heightWindow;
		float lastTime;
		float farThreshold;

};