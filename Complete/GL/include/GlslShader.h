#pragma once
#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <vector>

using namespace std;

class GlslShader {
public:
	/// ������ ��� ������� �� �������� ��������� ������ ������ � ���������:
	void use();
	/// �������� �������� �������� � ���������� ���������� � ������������ ������� � �������� � ���� ��������� ��������� GL:
	void setup(string vertexShaderPath, string fragmentShaderPath);

	/// �������� ������ � ID ��������� �������:
	GLuint& getProgramID() {
		return programID;
	}
	/// �������� � ������ ������� 4*4:
	void setUniformMatrix4fv(string name, GLfloat* value);
	void setUniform1f(string name, GLfloat value);
	void setUniform3f(string name, GLfloat x, GLfloat y, GLfloat z);


private:
	/// ������������������ ID ����� ��������� ��������� � GL, ��� ����� ������ �������������:
	GLuint programID;
	/// �������� ���� � ������������ ����� ������ ���������� ������ � ������ exe:
	string exePathWithName();
	/// ���� ����� �� ��� ��� ����� �����:
	string exePath();
};