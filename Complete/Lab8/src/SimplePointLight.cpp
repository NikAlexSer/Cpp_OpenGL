#include "SimplePointLight.h"

/// ��������� ������������:
void SimplePointLight::setup(GLFWwindow* window, float r, float g, float b) {
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	/// ������ �� 3 �������� ������� ����� ������������ 3 �������:
	static const GLfloat gVertexBufferData[] = {

		/// 1 �����:
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,

		/// 2 �����:
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		/// 3 �����:
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,

		/// 4 �����:
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,

		/// 5 �����:
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		/// 6 �����:
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
	};

	/// ������� ���������� OpenGL ����� � ��������� ��������� �� ���� � vertexBuffer:
	glGenBuffers(1, &vertexBuffer);
	/// �������� ������� vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// ������������� ���� ������� � OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);


	this->color = vec3(r, g, b);

	/// ���� ���� �� ������ �������: 
	static const GLfloat g�olorBufferData[] = {
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z,
		color.x,  color.y,  color.z
	};

	/// �������� ����� � ��������� ��� ����� ��� ��, ��� � � ���������� �������:
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g�olorBufferData), g�olorBufferData, GL_STATIC_DRAW);

	/// �������������� ������:
	shader.setup("data\\lightBox.vert", "data\\lightBox.frag");

	lastTime = glfwGetTime();
	this->window = window;
}


/// �������� � ����������� ����� ������� ����� ��������� ������ �������� ���������:
void SimplePointLight::update() {
	/// �������� ������� �����:
	double currentTime = glfwGetTime();

	/// ��������� ����� ���������� ��������:
	float deltaTime = float(currentTime - lastTime);

	const float speed = 1.7;

	/// ������� ���� �����/����:
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)		pos.y += deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)		pos.y -= deltaTime * speed;

	/// ������� ���� �����/������:
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)		pos.x -= deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)		pos.x += deltaTime * speed;

	/// ������� ���� ������/�����:
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)		pos.z += deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)		pos.z -= deltaTime * speed;


	setPosition(pos);

	lastTime = currentTime;
}

/// ������ ��������� ��������� ���������:
void SimplePointLight::draw(mat4 MVP) {

	shader.use();
	/// ��������� ������ � ���������:
	shader.setUniformMatrix4fv("MVP", &MVP[0][0]);
	shader.setUniformMatrix4fv("modelMatrix", &modelMatrix[0][0]);

	/// �������� ����� ����:
	glBindVertexArray(cubeVAO);
	/// ������ ����� ���������: �������
	glEnableVertexAttribArray(0);
	/// �������� ������� ��������� �����:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// ������������� ���������� ������ ��� ������������:
	glVertexAttribPointer(
		0,                  // ������� 0. ����������� ������ � ���� ���, �� ����� ������ ��������� � ������ � �������
		3,                  // ����������
		GL_FLOAT,           // ���
		GL_FALSE,           // ��������������� ��?
		0,                  // ���
		(void*)0            // �������� � ������
	);

	/// ������������� ����� ������:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		1,                                // �������. ��� ������ ������ ������ 1, �������, ����� ��������� �� ��������� � �������.
		3,                                // ������
		GL_FLOAT,                         // ���
		GL_FALSE,                         // ��������������� ��?
		0,                                // ���
		(void*)0                          //�������� � ������
	);

	glEnable(GL_DEPTH_TEST);
	/// ��������� ��������� ������� ����� � ������
	glDepthFunc(GL_LESS);
	/// ������ ����������� !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

