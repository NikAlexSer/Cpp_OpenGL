#include "SimpleCamera.h"

void SimpleCamera::setup(GLFWwindow* window, vec3 pos, float hAngle, float vAngle, float fov, float speed, float mouseSpeed){
	/// Выставляем параметры камеры:
	/// this-> означает что мы копируем значения переменных-аргументов функции в переменные класса камеры.
	this->pos			= pos;
	this->hAngle		= hAngle;
	this->vAngle		= vAngle;
	this->initialFov	= fov;
	this->speed			= speed;
	this->mouseSpeed	= mouseSpeed;
	this->window		= window;

	/// Получаем текущие размеры окна:
	glfwGetWindowSize(window,&widthWindow,&heightWindow);

	/// Скрываем курсор:
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	/// Двигаем мышь в центр экрана:
	glfwSetCursorPos(window,widthWindow/2, heightWindow/2);

}

void SimpleCamera::update(){

	/// Получаем координаты курсора:
	double xpos, ypos;
	glfwGetCursorPos(window,&xpos, &ypos);


	/// Двигаем мышь в центр экрана:
	glfwSetCursorPos(window,widthWindow/2, heightWindow/2);

	/// Получаем текущие время:
	double currentTime = glfwGetTime();

	/// Вычисляем время выполнения итерации:
	float deltaTime = float(currentTime - lastTime);

	/// Вычисляем горизонтальные и вертикальные углы:
	hAngle += mouseSpeed * deltaTime * float(widthWindow/2  - xpos);
	vAngle += mouseSpeed * deltaTime * float(heightWindow/2 - ypos);

	/// Вычисляем направление: преобразовываем сферические координаты в декартовы:
	vec3 direction(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle));


	/// Вектор «вправо»
	vec3 right = glm::vec3( sin(hAngle - 3.14f/2.0f), 0, cos(hAngle - 3.14f/2.0f));

	/// Вектор «вверх»: перпендикуляр к направлению и к «вправо»
	vec3 up = glm::cross(right, direction);



	/// Двигаемся вперед:
	if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS)		pos += direction * deltaTime * speed;	
	/// Двигаемся назад:
	if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS)		pos -= direction * deltaTime * speed;	
	/// Шаг вправо:
	if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS)		pos += right * deltaTime * speed;	
	/// Шаг влево:
	if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS)		pos -= right * deltaTime * speed;
	/// Движение вверх и вниз:
	if (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS)   	pos.y += deltaTime * speed;	
	if (glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS)   	pos.y -= deltaTime * speed;	
	
	

	// Матрица проекции : 45° Угол обзора, 4:3 соотношение сторон, дальность видимости от 0.1  до 100
	projectionMatrix = perspective(initialFov, (float)widthWindow / heightWindow, 0.1f, 100.0f);
	// Матрица камеры:
	viewMatrix = lookAt(pos, pos + direction, up);
	/// Определяем матрицу модели:
	modelMatrix = glm::mat4(1.0);
	/// Вычисляем результирующию матрицу:
	resultMVP = projectionMatrix * viewMatrix * modelMatrix;

	/// Запоминаем когда мы последний раз вызвали update:
	lastTime = currentTime;
}

