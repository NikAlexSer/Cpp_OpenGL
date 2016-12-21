#define _CRT_SECURE_NO_WARNINGS
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

#include <sstream>
#include <stddef.h>

using namespace std;
using namespace glm;

#include "loadTexture.h"
#include "tinyxml\tinyxml.h"

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
	vec3 tangent;
	vec3 biTangent;
};

class SimpleLoaderCollada3D {
	public:
		bool setup(string folder, bool useDiffuseMap = true, bool useSpecularMap = false, bool useNormalMap = false);
		void update(mat4 MVP);
		void update(mat4& model, mat4& view, mat4& projection);
		void draw(bool isSeparateCameraMatrix = false);
		void setScale(float x, float y, float z);
		void setPosition(float x, float y, float z);
		void setRotate(float angle, vec3 axis);

		void setShader(GlslShader *s) {
			shader = s;
		}

		void setLight(SimplePointLight* l) {
			light = l;
		}

		void enableSpecular(bool use) {
			useSpecularMap = use;
		}
		void setCameraPos(vec3 pos) {
			cameraPos = pos;
		}
		void setSpecularStrength(float value) {
			specularStrength = value;
		}
		void enableNormalMap(bool use) {
			useNormalMap = use;
		}

	private:
		mat4 globalMVP;
		mat4 modelMatrix;

		SimplePointLight* light;

		mat4 cameraModelMatrix;
		mat4 cameraViewMatrix;
		mat4 cameraProjectionMatrix;

		//вспомогательная функция для парсинга строки
		void split(const std::string &s, char delim, std::vector<string> &elems);
		std::vector<std::string> split(const std::string &s, char delim);
		
		template<typename T>
		void getPointsArrayFromStringTag(TiXmlElement* tag, vector <T>& pointsOut);

		//описание 3D модели
		vector <glm::vec3> coords;
		vector <glm::vec3> normals;
		vector <glm::vec2> uvCoords;
		//конечный массив собранных в правильном порядке вершин
		vector <Vertex> outVertices;

		GLuint VAO, VBO;
		GLuint diffuseTexture;
		GLuint specularTexture;
		GLuint normalTexture;
		GlslShader *shader;

		bool useDiffuseMap;
		bool useSpecularMap;
		bool useNormalMap;
		vec3 cameraPos;
		float specularStrength;
};