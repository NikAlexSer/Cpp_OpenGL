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
	/// ��������� ������������:
	void setup(GLFWwindow* window, float r, float g, float b);
	/// ������ ������� ��������� ����� � ���� ����, �������� ������� ������:		
	void draw(mat4 MVP);

	/// �������� � ����������� ����� ������� ����� ��������� ������ �������� ���������:
	void update();

	/// ������ �������:
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
	/// �������� ������� �������:
	mat4 modelMatrix;
	/// ��������� ����� � ������������:
	vec3	pos;
	/// ���� �����:
	vec3    color;
	/// ������������� Vertex Array Object ��� ������ ����:
	GLuint	cubeVAO;
	/// ������������� ���������� ������:
	GLuint vertexBuffer;
	/// ������������� ��������� ������:
	GLuint colorBuffer;

	/// ��������� ������:
	GlslShader shader;

	/// ��������� ����� ������ update():
	float lastTime;
	/// ���� ������ ����������, ��� ������� � ����������:
	GLFWwindow* window;
};
