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
	camera.setup(getWindow(), vec3(0, 0, 5), 3.14, 0.0f, 45.f, 3, 0.01, 1000);
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
}

//рендер одного кадра
void App::draw() {
	/// Выполняем очистки буфера цвета:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.5f, 0.0);
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);
	drawTextureCube();
	glBindVertexArray(0);

	/// Рисуем внешнее окружение:
	skyBox.draw();
}


void App::exit() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_FAILURE);
}


GLuint App::loadBmp(string fileName) {
	//Получаем путь до проги, конвертируем, ищем слэш,
	//копируем в переменную exePath и формируем полный путь
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string exePathWithName = string(buffer);
	string::size_type pos = exePathWithName.find_last_of("\\/");
	string exePath = string(exePathWithName).substr(0, pos);
	string fullPath = exePath + "\\data\\" + fileName;

	//открытие
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
	//Базовая фильтрация:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete data;
	return textureID;
}

GLuint App::loadDDS(string fileName) {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string exePathWithName = string(buffer);
	string::size_type pos = exePathWithName.find_last_of("\\/");
	string exePath = string(exePathWithName).substr(0, pos);
	string fullPath = exePath + "\\data\\" + fileName;

	//открытие
	FILE * file = fopen(fullPath.c_str(), "rb");
	if (!file) {
		cout << "error load image!" << endl;
		return 0;
	}

	char filecode[4];
	fread(filecode, 1, 4, file);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(file);
		cout << "extension file wrong!" << endl;
		cout << "error load image!" << endl;
		return 0;
	}
	unsigned char header[124];
	fread(&header, 124, 1, file);
	unsigned int height			= *(unsigned int*)&(header[8 ]);
	unsigned int width			= *(unsigned int*)&(header[12]);
	unsigned int linearSize		= *(unsigned int*)&(header[16]);
	unsigned int mipMapCount	= *(unsigned int*)&(header[24]);
	unsigned int fourCC			= *(unsigned int*)&(header[80]);

	unsigned char * bufferData;
	unsigned int bufsize;
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	bufferData = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(bufferData, 1, bufsize, file);
	fclose(file);

	//Сжатие и тд и тп
	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC) {
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(bufferData);
			return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, bufferData + offset);
		offset += size;
		width /= 2;
		height /= 2;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete bufferData;
	return textureID;
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
	//texture1 = loadBmp("container.bmp");
	texture1 = loadDDS("container.dds");
	texture2 = loadDDS("iron.dds");
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



