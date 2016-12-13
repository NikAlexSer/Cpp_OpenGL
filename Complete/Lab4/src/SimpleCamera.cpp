#include "SimpleCamera.h"

void SimpleCamera::setup(GLFWwindow* window, vec3 pos, float hAngle, float vAngle, float fov, float speed, float mouseSpeed) {
	//this копирует значения переменных функции в переменные класса
	this->pos			= pos;
	this->hAngle		= hAngle;
	this->vAngle		= vAngle;
	this->initialFov	= fov;
	this->speed			= speed;
	this->mouseSpeed	= mouseSpeed;
	this->window		= window;

	glfwGetWindowSize(window, &widthWindow, &heightWindow);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void SimpleCamera::update() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, widthWindow / 2, heightWindow / 2);

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	hAngle += mouseSpeed * deltaTime * float(widthWindow / 2 - xpos);
	vAngle += mouseSpeed * deltaTime * float(heightWindow / 2 - ypos);

	//вычисляем направление - преобразовываем сферические координаты в декартовы
	vec3 direction(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle));
	vec3 right = glm::vec3(sin(hAngle - 3.14f / 2.0f), 0, cos(hAngle - 3.14f / 2.0f));
	vec3 up = glm::cross(right, direction);

	//Вперед, назад, вправо, влево
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos += direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos -= +direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos += right * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos -= right * deltaTime * speed;

	projectionMatrix = perspective(initialFov, (float)widthWindow / heightWindow, 0.1f, 100.0f);
	viewMatrix = lookAt(pos, pos + direction, up);
	modelMatrix = glm::mat4(1.0);
	resultMVP = projectionMatrix * viewMatrix * modelMatrix;

	lastTime = currentTime;
}