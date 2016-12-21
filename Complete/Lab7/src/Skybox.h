#define _CRT_SECURE_NO_WARNINGS
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
#include "loadTexture.h"

using namespace std;
using namespace glm;

class SkyBox {
	public: 
		void setup(string folder, float sizeSkyBox);
		void setupTexture(string folder);
		void setupGeometry();
		void update(mat4 MVP, vec3 pos);
		void draw();
	private:
		GLuint textureID;
		GlslShader shader;
		mat4 MVP;
		mat4 modelMatrix;
		GLuint skyboxVAO, skyboxVBO;
		float sizeSkyBox;
};