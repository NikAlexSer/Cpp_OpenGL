#include "app.h"

void App::error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	const float moveStep = 0.05;

	//if (key == '1') getInstance().modelPos.x += moveStep;
	//if (key == '2') getInstance().modelPos.x -= moveStep;

	//if (key == '3') getInstance().modelPos.y += moveStep;
	//if (key == '4') getInstance().modelPos.y -= moveStep;

	//if (key == '5') getInstance().modelPos.z += moveStep;
	//if (key == '6') getInstance().modelPos.z -= moveStep;

	if (mods == 0 && (key == 'x' || key == 'X')) getInstance().angles.x += moveStep;
	if (mods == 0 && (key == 'y' || key == 'Y')) getInstance().angles.y += moveStep;
	if (mods == 0 && (key == 'z' || key == 'Z')) getInstance().angles.z += moveStep;

	if (mods == 1 && (key == 'x' || key == 'X')) getInstance().angles.x -= moveStep;
	if (mods == 1 && (key == 'y' || key == 'Y')) getInstance().angles.y -= moveStep;
	if (mods == 1 && (key == 'z' || key == 'Z')) getInstance().angles.z -= moveStep;
	
	/*if (key == '1') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if (key == '2') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}*/

	//if (key == '3') getInstance().mixFactor += 0.05;
	//if (key == '4') getInstance().mixFactor -= 0.05;
	if (getInstance().mixFactor < 0) getInstance().mixFactor = 0.0;
	if (getInstance().mixFactor > 1) getInstance().mixFactor = 1.0;
}

void App::mouse_callback(GLFWwindow* window, int a, int b, int c) {
	cout << "button a = " << a << endl;
	cout << "button b = " << b << endl;
	cout << "button c = " << c << endl;
}


void App::setupScene() {
	/// ������� �������� ���:
	//createTextureCube();

	/// �������������� ������:
	shader.setup("data\\simpleShader.vert", "data\\simpleShader.frag");

	/// �������������� ����� ��������� skyBox, ������ ������ ����� ��������� ��������, � ����� ��� ������:
	skyBox.setup("skybox", 500);
	float groundY = -1.0;

	shader3Dmodel.setup("data\\PhongSimpleLight.vert", "data\\PhongSimpleLight.frag");
	shader3Dmodel2.setup("data\\PhongSimpleLight2.vert", "data\\PhongSimpleLight2.frag");
	shader3Dmodel3.setup("data\\PhongSimpleLight3.vert", "data\\PhongSimpleLight3.frag");

	lightPoint.setup(window, 1, 1, 1);
	lightPoint.setPosition(-3, -0.5, 0);

	ground.setup("Plane001");
	ground.setPosition(-5, groundY - 0.05, 0);
	ground.setRotate(-90, vec3(1, 0, 0));
	ground.setScale(0.1, 0.1, 0.1);
	ground.setShader(&shader3Dmodel);
	ground.setLight(&lightPoint);

	house.setup("house", true, true, true);
	house.setPosition(-6, groundY, 1);
	house.setScale(0.03, 0.03, 0.03);
	house.setRotate(-90, vec3(1, 0, 0));
	house.setRotate(180, vec3(0, 0, 1));
	house.setShader(&shader3Dmodel3);
	house.setLight(&lightPoint);
	house.setSpecularStrength(2);

	teapot.setup("Teapot001");
	teapot.setPosition(-4.2, groundY, 5.2);
	teapot.setRotate(-90, vec3(1, 0, 0));
	teapot.setScale(0.02, 0.02, 0.02);
	teapot.setShader(&shader3Dmodel);
	teapot.setLight(&lightPoint);

	brickWall.setup("Plane002", true, false, true);
	brickWall.setPosition(-1, 0, 0);
	brickWall.setRotate(0, vec3(1, 0, 0));
	brickWall.setScale(0.012, 0.012, 0.012);
	brickWall.setShader(&shader3Dmodel3);
	brickWall.setLight(&lightPoint);
	brickWall.setSpecularStrength(1.0);
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
	camera.setup(getWindow(), vec3(0, 0, 10), 3.14, 0.0f, 45.f, 3, 0.01, 1000);
	testFbo.setup(1024, 768);
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

	skyBox.update(camera.getCameraMatrix(), camera.getPosition());
	lightPoint.update();

	house.update(camera.getCameraModelMatrix(), camera.getCameraViewMatrix(), camera.getCameraProjectionMatrix());
	house.setCameraPos(camera.getPosition()); 
	ground.update(camera.getCameraModelMatrix(), camera.getCameraViewMatrix(), camera.getCameraProjectionMatrix());
	teapot.update(camera.getCameraModelMatrix(), camera.getCameraViewMatrix(), camera.getCameraProjectionMatrix());
	brickWall.update(camera.getCameraModelMatrix(), camera.getCameraViewMatrix(), camera.getCameraProjectionMatrix());
	brickWall.setCameraPos(camera.getPosition());

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		testFbo.modeFx = 0;
		testFbo.param0 = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		testFbo.modeFx = 1;
		cout << testFbo.modeFx << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) testFbo.modeFx = 2;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) testFbo.modeFx = 3;

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		testFbo.modeFx = 4;
		if (testFbo.modeFx != 5) {
			testFbo.startTimeFx = glfwGetTime();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		testFbo.param2 = testFbo.param0;
		testFbo.modeFx = 5;
		if (testFbo.modeFx != 4) {
			testFbo.startTimeFx = glfwGetTime();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		testFbo.modeFx = 6;
		testFbo.startTimeFx = glfwGetTime();
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) testFbo.param0 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) testFbo.param0 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) testFbo.param1 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) testFbo.param1 -= 0.01;
}

//������ ������ �����
void App::draw() {
	/// ��������� ������� ������ �����:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.5f, 0.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	//drawTextureCube();
	testFbo.begin();
	ground.draw(true);
	house.draw(true);
	teapot.draw(true);
	brickWall.draw(true);
	lightPoint.draw(camera.getCameraMatrix());
	skyBox.draw();

	testFbo.end();
	testFbo.draw();
}


void App::exit() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_FAILURE);
}


void App::createTextureCube() {
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

	/// ��������� ��������, � ���������� � ������������� � texture1:
	//texture1 = loader.loadBmp("container.bmp");
	texture1 = loader.loadDDS("container.dds");
	texture2 = loader.loadDDS("iron.dds");
	/// ��������� ����������:
	mixFactor = 0;

	/// �� ������ ������� �� ��� UV ����������:
	static const GLfloat gUVBufferData[] = {

		/// ��������������� ���������� ��� 1 ����� ����:
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		/// ��������������� ���������� ��� 2 ����� ����:
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

	};

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gUVBufferData), gUVBufferData, GL_STATIC_DRAW);
}



void App::drawTextureCube() {

	shader.use();
	/// �������� ������� ������:
	shader.setUniformMatrix4fv("MVP", &camera.getCameraMatrix()[0][0]);
	/// �������� ����� ����:
	glBindVertexArray(cubeVAO);
	/// �������� 0 ������� ������:
	glEnableVertexAttribArray(0);
	/// ������ ��� ���� �������� ����� � ������������, � �������� ��� � �������� ��������:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // ������� 0. ����������� ������ � ���� ���, �� ����� ������ ��������� � ������ � �������
		3,                  // ����������
		GL_FLOAT,           // ���
		GL_FALSE,           // ��������������� ��?
		0,                  // ���
		(void*)0            // �������� � ������
	);

	/// ���� �����, ������ ��� ���������� ���������:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // �������. ��� ������ ������ ������ 1, �������, ����� ��������� �� ��������� � �������.
		2,                                // ������
		GL_FLOAT,                         // ���
		GL_FALSE,                         // ��������������� ��?
		0,                                // ���
		(void*)0                          //�������� � ������
	);

	/// 1 ��������:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture1"), 0);
	/////// 2 ��������:
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture2"), 1);
	shader.setUniform1f("mixFactor", getInstance().mixFactor);
	/// ������ �������������� ���:
	glDrawArrays(GL_TRIANGLES, 0, 36);
}



