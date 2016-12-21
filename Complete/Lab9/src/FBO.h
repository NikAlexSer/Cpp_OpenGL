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
#include <vector>
#include "GlslShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;


struct fsqVertex {
	vec3 position;
	vec2 texcoord;
};

class FBO {
	public:
		FBO(){}

		~FBO() {
			glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &textureRGBA);
			glDeleteFramebuffersEXT(1, &fboID);
		}

		void draw();
		void begin();
		void end();
		void setup(int w, int h);

		GlslShader* getShader() {
			return &shader;
		}
		GLuint getTextureRGBA() {
			return textureRGBA;
		}

		int modeFx;
		float param0;
		float param1;
		float param2;

		int startTimeFx;

	private:
		int width, height;

		void createPlane();

		GLuint createTextureRGBA8(int w, int h, bool createMipMap);

		GLuint fboID;
		GLuint textureRGBA;
		GLuint depthBuffer;
		GLuint fsqVAO, fsqVBO;
		GlslShader shader;
};