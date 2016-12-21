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
	
	if (key == '1') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if (key == '2') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if (key == '3') getInstance().mixFactor += 0.05;
	if (key == '4') getInstance().mixFactor -= 0.05;
	if (getInstance().mixFactor < 0) getInstance().mixFactor = 0.0;
	if (getInstance().mixFactor > 1) getInstance().mixFactor = 1.0;
}

void App::mouse_callback(GLFWwindow* window, int a, int b, int c) {
	cout << "button a = " << a << endl;
	cout << "button b = " << b << endl;
	cout << "button c = " << c << endl;
}


void App::setupScene() {
	/// Создаем тестовый куб:
	createTextureCube();

	/// Инициализируем шейдер:
	shader.setup("data\\simpleShader.vert", "data\\simpleShader.frag");

	/// Инициализируем новый компонент skyBox, задаем откуда будут грузиться текстуры, а также его размер:
	skyBox.setup("skybox", 500);

	model3D.setup("house");
	model3D.setScale(0.03, 0.03, 0.03);
	model3D.setRotate(-90, vec3(1, 0, 0));
	model3D.setRotate(140, vec3(0, 0, 1));
	model3D.setPosition(-200, 0, 0);
}


void App::setup(int w, int h, string captionWindow, bool useAntiAliasing) {
	glfwSetErrorCallback(error_callback);

	//инициализация GLFW
	if (!glfwInit()) std::exit(EXIT_FAILURE);
	//вклюбчаем 4-х кратное сглаживание для устранения эффекта зазубренности изображдения
	if (useAntiAliasing) glfwWindowHint(GLFW_SAMPLES, 4);

	//Создаем GLFW окно с записью в память местонахождения
	window = glfwCreateWindow(w, h, captionWindow.c_str(), NULL, NULL);

	//Если окно не создано, то есть указатель содержит NULL то терминате
	if (!window) {
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	//Подключаем контекст, колво экранных обновлений, отслеживание событий
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	//Инит GLEW
	glewInit();
	//Инит геометрию и все что с этим связано
	setupScene();
	//Инит положение, масштаб, поворот и ось поворота
	getInstance().modelPos.x = 0;
	getInstance().modelPos.y = 0;
	getInstance().modelPos.z = 0;
	getInstance().modelScale.x = 1;
	getInstance().modelScale.y = 1;
	getInstance().modelScale.z = 1;
	getInstance().angles = vec3(0, 0, 0);
	getInstance().axis = vec3(0, 0, 1);

	//Инит камера
	camera.setup(getWindow(), vec3(0, 0, 10), 3.14, 0.0f, 45.f, 3, 0.01, 1000);
}

//обновление всей логики и переменных
void App::update() {
	camera.update();

	float fovRad = 45.0f * (3.14f / 180.0f);
	//Задаем проекционную матрицу: угол обзора, соотношение, дальность вида
	mat4 projection = perspective(fovRad, 4.0f / 3.0f, 0.1f, 100.0f);
	//Матрица камеры
	mat4 view = lookAt(
		vec3(0, 0, 10),
		vec3(0.0f,0.0f,0.0f),
		vec3(0,1,0)
	);
	//Выполняем масштаб, поворот и смещение матрницы модели
	model = mat4(1.0f);
	model = scale(model, getInstance().modelScale);
	model = glm::rotate(model, getInstance().angles.x, vec3(1, 0, 0));
	model = glm::rotate(model, getInstance().angles.y, vec3(0, 1, 0));
	model = glm::rotate(model, getInstance().angles.z, vec3(0, 0, 1));
	model = translate(model, getInstance().modelPos);

	//Глобальная матрицы
	globalMVP = projection * view * model;

	skyBox.update(camera.getCameraMatrix(), camera.getPosition());

	model3D.update(camera.getCameraMatrix());
}

//рендер одного кадра
void App::draw() {
	/// Выполняем очистки буфера цвета:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.5f, 0.0);
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);
	drawTextureCube();
	model3D.draw();
	glBindVertexArray(0);

	/// Рисуем внешнее окружение:
	skyBox.draw();
}


void App::exit() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_FAILURE);
}


void App::createTextureCube() {
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	/// Массив из 3 векторов которые будут представлять 3 вершины:
	static const GLfloat gVertexBufferData[] = {

		/// 1 Грань:
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,

		/// 2 Грань:
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		/// 3 Грань:
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,

		/// 4 Грань:
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,

		/// 5 Грань:
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		/// 6 Грань:
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
	};

	/// Сначала генерируем OpenGL буфер и сохраняем указатель на него в vertexBuffer:
	glGenBuffers(1, &vertexBuffer);
	/// Выбираем текущим vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// Предоставляем наши вершины в OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	/// Загружаем текстуру, и записываем её идентификатор в texture1:
	//texture1 = loader.loadBmp("container.bmp");
	texture1 = loader.loadDDS("container.dds");
	texture2 = loader.loadDDS("iron.dds");
	/// Коэфицент смешивания:
	mixFactor = 0;

	/// На каждую вершину по две UV координаты:
	static const GLfloat gUVBufferData[] = {

		/// Текстурирование координаты для 1 грани куба:
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		/// Текстурирование координаты для 2 грани куба:
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
	/// Передаем матрицу камеры:
	shader.setUniformMatrix4fv("MVP", &camera.getCameraMatrix()[0][0]);
	/// Выбираем буфер куба:
	glBindVertexArray(cubeVAO);
	/// Выбираем 0 атрибут вершин:
	glEnableVertexAttribArray(0);
	/// Задаем под него вершиный буфер с координатами, и передаем его в качестве атрибута:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // Атрибут 0. Сакрального смысла в нуле нет, но число должно совпадать с числом в шейдере
		3,                  // количество
		GL_FLOAT,           // тип
		GL_FALSE,           // нормализировано ли?
		0,                  // шаг
		(void*)0            // смещение в буфере
	);

	/// Тоже самое, только для текстурных координат:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере.
		2,                                // размер
		GL_FLOAT,                         // тип
		GL_FALSE,                         // нормализировано ли?
		0,                                // шаг
		(void*)0                          //смещение в буфере
	);

	/// 1 текстура:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture1"), 0);
	/////// 2 текстура:
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture2"), 1);
	shader.setUniform1f("mixFactor", getInstance().mixFactor);
	/// Рисуем треугольниками куб:
	glDrawArrays(GL_TRIANGLES, 0, 36);
}



