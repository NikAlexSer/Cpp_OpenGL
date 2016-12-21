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
#include <sstream>
#include <stddef.h>  
using namespace std;
using namespace glm;

#include "loadTexture.h"
#include "tinyxml\tinyxml.h"

struct Vertex {
    /// ���������� �������:
    vec3 position;
    /// �������:
    vec3 normal;
    // ����������� ����������:
    vec2 texCoords;
};


/// ����� �������� 3D ������ � ������� collada
/// ���� ��������� ���� �������� ����� ���������(mesh) �� �������������
/// ����� ������ �� ����� ���������� ������, ���������� �������� ������ � ���������� ���������� ������
/// � ����� ������� ������������� ������� ����� ��������� ������� ��������� ������, ���������� ���������, � ��������� ��������:
class SimpleLoaderCollada3D {
	public:
		/// ��������� ����� � 3D �������:
		bool setup(string folder);
		/// ��������� ��������� ������, � ������� � ��������:
		void update(mat4 MVP);
		void draw();

		void setScale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotate(float angle, vec3 axis);

	private:	
		mat4 globalMVP;
		mat4 modelMatrix;

		/// ��������������� ������� ��� �������� ������:
		void split(const std::string &s, char delim, std::vector<string> &elems);
		std::vector<std::string> split(const std::string &s, char delim);

		/// �������� ������ 3D/2D ��������� �� ���� ���������� ����� ����������� ���������:
		template<typename T> 
		void getPointsArrayFromStringTag(TiXmlElement* tag, vector <T>& pointsOut);


		/// �������� �����:
		vector <glm::vec3> coords;
		vector <glm::vec3> normals;
		vector <glm::vec2> uvCoords;
		/// �������� ������ ��������� � ���������� ������� ������:
		vector <Vertex> outVertices;

		/// ID ���������� ������:
		GLuint VAO, VBO;
		/// ����������� ��������:
		GLuint diffuseTexture;
		/// ��������� ������:
		GlslShader shader;
};