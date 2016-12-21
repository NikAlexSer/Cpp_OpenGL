#include "Skybox.h"

void SkyBox::setupTexture(string folder) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	unsigned w, h, f, s;
	loaderImage loader;
	///Справа слева сверху снизу сзади спереди
	unsigned char * rightPixels = loader.loadPixelsFromDDSFile(folder + "\\right.dds", w, h, f, s);
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, f, w, h, 0, s, rightPixels);

	unsigned char * leftPixels = loader.loadPixelsFromDDSFile(folder + "\\left.dds", w, h, f, s);
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, f, w, h, 0, s, leftPixels);

	unsigned char * topPixels = loader.loadPixelsFromDDSFile(folder + "\\top.dds", w, h, f, s);
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, f, w, h, 0, s, topPixels);

	unsigned char * bottomPixels = loader.loadPixelsFromDDSFile(folder + "\\bottom.dds", w, h, f, s);
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, f, w, h, 0, s, bottomPixels);

	unsigned char * backPixels = loader.loadPixelsFromDDSFile(folder + "\\back.dds", w, h, f, s);
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, f, w, h, 0, s, backPixels);

	unsigned char * frontPixels = loader.loadPixelsFromDDSFile(folder + "\\front.dds", w, h, f, s);
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, f, w, h, 0, s, frontPixels);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::setupGeometry() {
	GLfloat skyboxVertices[] = {
		/// Координаты вершин граней куба:        
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	/// Инициализируем геометрию skyBox:
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	/// Передаем вершины в качестве атрибута для VertexArray:
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}


void SkyBox::setup(string folder, float sizeSkyBox) {
	setupGeometry();
	setupTexture(folder);
	shader.setup("data\\skyBox.vert", "data\\skyBox.frag");
	this->sizeSkyBox = sizeSkyBox;
}


void SkyBox::draw() {
	glDepthFunc(GL_LEQUAL);
	shader.use();
	shader.setUniformMatrix4fv("MVP", &MVP[0][0]);
	shader.setUniformMatrix4fv("modelMatrix", &modelMatrix[0][0]);
	shader.setUniform1f("sizeSkyBox", sizeSkyBox);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}


void SkyBox::update(mat4 MVP, vec3 pos) {
	this->MVP = MVP;
	modelMatrix = mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, pos);
}