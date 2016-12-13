#include "app.h"

void App::error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	const float moveStep = 0.05;

	if (key == '1') getInstance().modelPos.x += moveStep;
	if (key == '2') getInstance().modelPos.x -= moveStep;

	if (key == '3') getInstance().modelPos.y += moveStep;
	if (key == '4') getInstance().modelPos.y -= moveStep;

	if (key == '5') getInstance().modelPos.z += moveStep;
	if (key == '6') getInstance().modelPos.z -= moveStep;

	if (mods == 0 && (key == 'x' || key == 'X')) getInstance().angles.x += moveStep;
	if (mods == 0 && (key == 'y' || key == 'Y')) getInstance().angles.y += moveStep;
	if (mods == 0 && (key == 'z' || key == 'Z')) getInstance().angles.z += moveStep;

	if (mods == 1 && (key == 'x' || key == 'X')) getInstance().angles.x -= moveStep;
	if (mods == 1 && (key == 'y' || key == 'Y')) getInstance().angles.y -= moveStep;
	if (mods == 1 && (key == 'z' || key == 'Z')) getInstance().angles.z -= moveStep;
}

void App::mouse_callback(GLFWwindow* window, int a, int b, int c) {
	cout << "button a = " << a << endl;
	cout << "button b = " << b << endl;
	cout << "button c = " << c << endl;
}


void App::setupScene() {
	//������� VAO � �������� ��� �������
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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



	//������� ���������� ������ ����� � ��������� ��������� �� ���� � vertexBuffer
	glGenBuffers(1, &vertexBuffer);

	//�������� ������� vertexBuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	//������������� ���� ������� � OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
	texture1 = loadBmp("container.bmp");

	static const GLfloat gUVBufferData[] = {
		//���������� ���������� ��� 1 ����� ����
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		//���������� ���������� ��� 2 ����� ����
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		//���������� ���������� ��� 3 ����� ����
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		//���������� ���������� ��� 4 ����� ����
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		//���������� ���������� ��� 5 ����� ����
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		//���������� ���������� ��� 6 ����� ����
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gUVBufferData), gUVBufferData, GL_STATIC_DRAW);

}

void App::setup(int w, int h, string captionWindow, bool useAntiAliasing) {
	glfwSetErrorCallback(error_callback);

	//������������� GLFW
	if (!glfwInit()) std::exit(EXIT_FAILURE);
	//��������� 4-� ������� ����������� ��� ���������� ������� ������������� ������������
	if (useAntiAliasing) glfwWindowHint(GLFW_SAMPLES, 4);

	//������� GLFW ���� � ������� � ������ ���������������
	window = glfwCreateWindow(w, h, captionWindow.c_str(), NULL, NULL);

	//���� ���� �� �������, �� ���� ��������� �������� NULL �� ���������
	if (!window) {
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	//���������� ��������, ����� �������� ����������, ������������ �������
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	//���� GLEW
	glewInit();
	//���� ��������� � ��� ��� � ���� �������
	setupScene();
	shader.setup("simpleShader.vert", "simpleShader.frag");
	//���� ���������, �������, ������� � ��� ��������
	getInstance().modelPos.x = 0;
	getInstance().modelPos.y = 0;
	getInstance().modelPos.z = 0;
	getInstance().modelScale.x = 1;
	getInstance().modelScale.y = 1;
	getInstance().modelScale.z = 1;
	getInstance().angles = vec3(0, 0, 0);
	getInstance().axis = vec3(0, 0, 1);

	//���� ������
	camera.setup(getWindow(), vec3(0, 0, 5), 3.14, 0.0f, 45.f, 3, 0.01);
}

//���������� ���� ������ � ����������
void App::update() {
	camera.update();

	float fovRad = 45.0f * (3.14f / 180.0f);
	//������ ������������ �������: ���� ������, �����������, ��������� ����
	mat4 projection = perspective(fovRad, 4.0f / 3.0f, 0.1f, 100.0f);
	//������� ������
	mat4 view = lookAt(
		vec3(0, 0, 10),
		vec3(0.0f,0.0f,0.0f),
		vec3(0,1,0)
	);
	//��������� �������, ������� � �������� �������� ������
	model = mat4(1.0f);
	model = scale(model, getInstance().modelScale);
	model = glm::rotate(model, getInstance().angles.x, vec3(1, 0, 0));
	model = glm::rotate(model, getInstance().angles.y, vec3(0, 1, 0));
	model = glm::rotate(model, getInstance().angles.z, vec3(0, 0, 1));
	model = translate(model, getInstance().modelPos);

	//���������� �������
	globalMVP = projection * view * model;
}

//������ ������ �����
void App::draw() {
	//������� ������ �����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.5f, 0.0);

	//������ ����� ���������: �������
	glEnableVertexAttribArray(0);
	//�������� ������� ��������� �����
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//��������� ������ � ���������
	shader.setUniformMatrix4fv("MVP", &camera.getCameraMatrix()[0][0]);

	//�������� �������� � ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture1"), 0);

	shader.use();

	glEnable(GL_DEPTH_TEST);
	/// ��������� ��������� ������� ����� � ������
	glDepthFunc(GL_LESS);


	glDrawArrays(GL_TRIANGLES, 0, 12*3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void App::exit() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_FAILURE);
}


GLuint App::loadBmp(string fileName) {
	//�������� ���� �� �����, ������������, ���� ����,
	//�������� � ���������� exePath � ��������� ������ ����
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PACKAGE_NAME);
	string exePathWithName = string(buffer);
	string::size_type pos = exePathWithName.find_last_of("\\/");
	string exePath = string(exePathWithName).substr(0, pos);
	string fullPath = exePath + "\\data\\" + fileName;

	//��������
	FILE * file = fopen(fullPath.c_str(), "rb");
	if (!file) {
		cout << "error load image!" << endl;
		return 0;
	}

	unsigned char header[54];
	if (fread(header, 1, 54, file) != 54) {
		cout << "error read header!" << endl;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		cout << "not a correct bmp file" << endl;
		return 0;
	}
	unsigned int dataPos = *(int*)&(header[0x0A]);
	unsigned int imageSize = *(int*)&(header[0x22]);
	unsigned int width = *(int*)&(header[0x12]);
	unsigned int height = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	unsigned char * data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	//������� ����������:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete data;
	return textureID;
}


