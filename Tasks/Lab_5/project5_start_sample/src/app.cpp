#include "app.h"

/// Данные функции вызываются когда происходят события нажатия на клавиатуру, события с мышью и возмыжные неполадки:
 void App::error_callback(int error, const char* description){
	fputs(description, stderr);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}



void App::mouse_callback(GLFWwindow* window, int a, int b, int c){
    cout << "button a = " << a  << endl;
    cout << "button b = " << b  << endl;
    cout << "button c = " << c  << endl;
}


/// Инициализируем геометрию и все что с этим может быть связано:
void App::setupScene(){
	/// Создаем VAO и выбираем его текущим:
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/// Массив из 3 векторов которые будут представлять 3 вершины:
	static const GLfloat gVertexBufferData[] = {
	   -1.0f,-1.0f,-1.0f, // начало 1 треугольника
	   -1.0f,-1.0f, 1.0f,
	   -1.0f, 1.0f, 1.0f, // конец первого треугольника
	   1.0f, 1.0f,-1.0f, // начало второго треугольника
	   -1.0f,-1.0f,-1.0f,
	   -1.0f, 1.0f,-1.0f, // конец второго треугольника
	   1.0f,-1.0f, 1.0f,
	   -1.0f,-1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	   -1.0f,-1.0f,-1.0f,
	   -1.0f,-1.0f,-1.0f,
	   -1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f, 1.0f,
	   -1.0f,-1.0f, 1.0f,
	   -1.0f,-1.0f,-1.0f,
	   -1.0f, 1.0f, 1.0f,
	   -1.0f,-1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f,-1.0f,
	   -1.0f, 1.0f,-1.0f,
	   1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f,-1.0f,
	   -1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f
	};

	/// Сначала генерируем OpenGL буфер и сохраняем указатель на него в vertexBuffer:
	glGenBuffers(1, &vertexBuffer);
	/// Выбираем текущим vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// Предоставляем наши вершины в OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	/// Добавляем цвета для куба:

	/// Один цвет на каждую вершину: 
	static const GLfloat gСolorBufferData[] = {
		   0.583f,  0.771f,  0.014f,
		   0.609f,  0.115f,  0.436f,
		   0.327f,  0.483f,  0.844f,
		   0.822f,  0.569f,  0.201f,
		   0.435f,  0.602f,  0.223f,
		   0.310f,  0.747f,  0.185f,
		   0.597f,  0.770f,  0.761f,
		   0.559f,  0.436f,  0.730f,
		   0.359f,  0.583f,  0.152f,
		   0.483f,  0.596f,  0.789f,
		   0.559f,  0.861f,  0.639f,
		   0.195f,  0.548f,  0.859f,
		   0.014f,  0.184f,  0.576f,
		   0.771f,  0.328f,  0.970f,
		   0.406f,  0.615f,  0.116f,
		   0.676f,  0.977f,  0.133f,
		   0.971f,  0.572f,  0.833f,
		   0.140f,  0.616f,  0.489f,
		   0.997f,  0.513f,  0.064f,
		   0.945f,  0.719f,  0.592f,
		   0.543f,  0.021f,  0.978f,
		   0.279f,  0.317f,  0.505f,
		   0.167f,  0.620f,  0.077f,
		   0.347f,  0.857f,  0.137f,
		   0.055f,  0.953f,  0.042f,
		   0.714f,  0.505f,  0.345f,
		   0.783f,  0.290f,  0.734f,
		   0.722f,  0.645f,  0.174f,
		   0.302f,  0.455f,  0.848f,
		   0.225f,  0.587f,  0.040f,
		   0.517f,  0.713f,  0.338f,
		   0.053f,  0.959f,  0.120f,
		   0.393f,  0.621f,  0.362f,
		   0.673f,  0.211f,  0.457f,
		   0.820f,  0.883f,  0.371f,
		   0.982f,  0.099f,  0.879f
	};

	/// Создайте буфер и заполните его точно так же, как и в предыдущих случаях:
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gСolorBufferData), gСolorBufferData, GL_STATIC_DRAW);
}

/// Выполняем инициализацию:
void App::setup(int w, int h, string captionWindow, bool useAntiAliasing){
	/// Подключаем функцию error_callback для отслеживания возможных ошибок связанных с GL или
	/// конкретно библиотекой GLFW
	glfwSetErrorCallback(error_callback);

	/// Выполняем инициализацию библиотеки GLFW:
	if (!glfwInit()) std::exit(EXIT_FAILURE);
	/// Включаем 4 кратное сглаживания для устранения эффекта зазубренности изображения:
	if (useAntiAliasing) glfwWindowHint(GLFW_SAMPLES,4);

	/// Создаем GLFW окно с нужным размером и заголовком, записываем указатель на область памяти где 
	/// где находится данное окно:
	window = glfwCreateWindow(w, h, captionWindow.c_str(), NULL, NULL);
	
	/// Если окно не создано, то есть указатель содержит NULL - аварийно завершаем наше приложение:
    if (!window){
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

	/// Подключаем выполняет контекст OpenGL для окна window:
    glfwMakeContextCurrent(window);
	/// Задаем количество экранных  обновлений прежде:
    glfwSwapInterval(1);
	/// Подключаем отслеживания событий мыши и клавиатуры:
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

	/// Инициализируем GLEW:
	glewInit();
	/// Инициализируем геометрию и все что с этим может быть связано:
	setupScene();
	/// Инициализируем шейдер:
	shader.setup("simpleShader.vert","simpleShader.frag");
	/// Инициализируем положение, масштаб, угол поворота и ось поворота:
	getInstance().modelPos.x    = 0;
	getInstance().modelPos.y    = 0;
	getInstance().modelPos.z    = 0;				
	getInstance().modelScale.x  = 1;
	getInstance().modelScale.y  = 1;
	getInstance().modelScale.z  = 1;
	getInstance().angles		= vec3(0,0,0);

	/// Задаем параметры камеры:
	camera.setup(getWindow(),vec3(0,0,5),3.14,0.0f,45.f,3,0.01);
}


/// Выполняем обновление всех C++ переменной - всей логики приложения:
void App::update(){
	/// обновляем камеру:
	camera.update();
}


/// Выполняем рендеринг одного кадра:
void App::draw(){
	/// Выполняем очистки буфера цвета:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.5f,0.0);

	/// Первый буфер атрибутов: вершины
	glEnableVertexAttribArray(0);
	/// Выбираем текущий вершинный буфер:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// Интерпритация вершинного буфера при визуализации:
	glVertexAttribPointer(
		0,                  // Атрибут 0. Сакрального смысла в нуле нет, но число должно совпадать с числом в шейдере
		3,                  // количество
		GL_FLOAT,           // тип
		GL_FALSE,           // нормализировано ли?
		0,                  // шаг
		(void*)0            // смещение в буфере
	);


	/// Конфигурируем буфер цветов:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		1,                                // атрибут. Нет особых причин писать 1, главное, чтобы совпадало со значением в шейдере.
		3,                                // размер
		GL_FLOAT,                         // тип
		GL_FALSE,                         // нормализировано ли?
		0,                                // шаг
		(void*)0                          //смещение в буфере
	);


	/// Применяем шейдер к геометрии:
	shader.setUniformMatrix4fv("MVP",&camera.getCameraMatrix()[0][0]);
	shader.use();

	glEnable(GL_DEPTH_TEST);
	/// Принимать фрагменты которые ближе к камере
	glDepthFunc(GL_LESS);

	/// Рисуем треугольник !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); 
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

/// Выход из приложения:
void App::exit(){
	/// Если цикл не выполняется, значит окно закрывается и нужно его удалить из памяти:
    glfwDestroyWindow(window);
	/// Также нужно завершить работу с glfw, высвободить все ресурсы связанные с OpenGL, окном и прочее:
    glfwTerminate();
	/// Выйти из приложении сообщив о нормальном(штатном) завершении работы программы:
    std::exit(EXIT_SUCCESS);
}

