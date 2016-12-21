#include "FBO.h"

void FBO::setup(int w, int h) {
	width = w;
	height = h;

	textureRGBA = createTextureRGBA8(w, h, false);
	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureRGBA, 0);


	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	bool status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (!status) cout << "error setup fbo!" << endl;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	createPlane();
	shader.setup("data\\post_fx.vert", "data\\post_fx.frag");
	modeFx = 0;
	param0 = 0;
	param1 = 0;
	startTimeFx = 0;
}

GLuint FBO::createTextureRGBA8(int w, int h, bool createMipMap) {
	GLuint result;
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (createMipMap) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
	else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	return result;
}


void FBO::createPlane() {
	fsqVAO = 0;
	fsqVBO = 0;

	const fsqVertex fsqVertices[6] = {
		{ vec3(-1.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f) },
		{ vec3(1.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f) },
		{ vec3(-1.0f,  1.0f, 0.0f), vec2(0.0f, 1.0f) },
		{ vec3(1.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f) },
		{ vec3(1.0f,  1.0f, 0.0f), vec2(1.0f, 1.0f) },
		{ vec3(-1.0f,  1.0f, 0.0f), vec2(0.0f, 1.0f) },
	};

	glGenVertexArrays(1, &fsqVAO);
	glBindVertexArray(fsqVAO);
	glGenBuffers(1, &fsqVBO);

	glBindBuffer(GL_ARRAY_BUFFER, fsqVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(fsqVertex), fsqVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(fsqVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fsqVertex), (GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(1);
}


void FBO::begin() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void FBO::end() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void FBO::draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureRGBA);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "colorTexture"), 0);

	if (modeFx >= 2) param0 = glfwGetTime() - startTimeFx;

	shader.setUniform1i("mode", modeFx);
	shader.setUniform1f("param0", param0);
	shader.setUniform1f("param1", param1);
	shader.setUniform1f("param2", param2);

	glViewport(0, 0, width, height);
	glBindVertexArray(fsqVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

