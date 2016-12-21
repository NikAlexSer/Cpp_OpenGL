#include "SimplePointLight.h"

/// Выполняем инциализацию:
void SimplePointLight::setup(GLFWwindow* window, float r, float g, float b) {
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


	this->color = vec3(r, g, b);

	/// Один цвет на каждую вершину: 
	static const GLfloat gСolorBufferData[] = {
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

	/// Создайте буфер и заполните его точно так же, как и в предыдущих случаях:
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gСolorBufferData), gСolorBufferData, GL_STATIC_DRAW);

	/// Инициализируем шейдер:
	shader.setup("data\\lightBox.vert", "data\\lightBox.frag");

	lastTime = glfwGetTime();
	this->window = window;
}


/// Работаем с клавиатурой чтобы сделать более наглядным работу источник освещения:
void SimplePointLight::update() {
	/// Получаем текущие время:
	double currentTime = glfwGetTime();

	/// Вычисляем время выполнения итерации:
	float deltaTime = float(currentTime - lastTime);

	const float speed = 1.7;

	/// Двигаем свет вверх/вниз:
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)		pos.y += deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)		pos.y -= deltaTime * speed;

	/// Двигаем свет влево/вправо:
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)		pos.x -= deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)		pos.x += deltaTime * speed;

	/// Двигаем свет вперед/назад:
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)		pos.z += deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)		pos.z -= deltaTime * speed;


	setPosition(pos);

	lastTime = currentTime;
}

/// Рисуем положение источника освещения:
void SimplePointLight::draw(mat4 MVP) {

	shader.use();
	/// Применяем шейдер к геометрии:
	shader.setUniformMatrix4fv("MVP", &MVP[0][0]);
	shader.setUniformMatrix4fv("modelMatrix", &modelMatrix[0][0]);

	/// Выбираем буфер куба:
	glBindVertexArray(cubeVAO);
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

	glEnable(GL_DEPTH_TEST);
	/// Принимать фрагменты которые ближе к камере
	glDepthFunc(GL_LESS);
	/// Рисуем треугольник !
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
}

