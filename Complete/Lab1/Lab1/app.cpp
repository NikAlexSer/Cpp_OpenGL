/// Подключаем файл app.h чтобы app.cpp видел небходимые внешние файлы библиотеки GLFW:
#include "app.h"

/// Данные функции вызываются когда происходят события нажатия на клавиатуру, события с мышью и возмыжные неполадки:
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, int a, int b, int c) {
	cout << "button a = " << a << endl;
	cout << "button b = " << b << endl;
	cout << "button c = " << c << endl;
}

/// Это основная функция main() которая является точкой входа в наше приложения, откуда мы уже запускаем все другие подсистемы нашего 
/// приложения:
int main() {
	/// Указатель на используемое окно:
	GLFWwindow* window;
	/// Подключаем функцию error_callback для отслеживания возможных ошибок связанных с GL или
	/// конкретно библиотекой GLFW
	glfwSetErrorCallback(error_callback);
	/// Выполняем инициализацию библиотеки GLFW:
	if (!glfwInit())
		exit(EXIT_FAILURE);
	/// Включаем 4 кратное сглаживания для устранения эффекта зазубренности изображения:
	glfwWindowHint(GLFW_SAMPLES, 4);

	/// Включаем продвинутую свежею версию OpenGL 3.3:
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// Создаем GLFW окно с нужным размером и заголовком, записываем указатель на область памяти где 
	/// где находится данное окно:
	window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);

	/// Если окно не создано, то есть указатель содержит NULL - аварийно завершаем наше приложение:
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/// Подключаем выполняет контекст OpenGL для окна window:
	glfwMakeContextCurrent(window);
	/// Задаем количество экранных  обновлений прежде:
	glfwSwapInterval(1);
	/// Подключаем отслеживания событий мыши и клавиатуры:
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	/// Запускаем цикл обновления картинки в окне:
	/// Выполняем цикл до тех пор пока окно не закрывается:
	while (!glfwWindowShouldClose(window))
	{
		/// Коэфицент соотношения сторон окна:
		float ratio;
		/// Ширина и высота окна:
		int width, height;
		/// В реальном времени получаем размер окна:
		glfwGetFramebufferSize(window, &width, &height);
		/// Получаем позицию курсора:
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		/// Вычисляем коэфицент соотношения сторон:
		ratio = width / (float)height;
		/// Далее идут базовые GL функции:
		/// Задаем двумерную область вывода изображения на экране:
		/// То есть та область где будет визуализация изображения OpenGL:
		glViewport(0, 0, width, height);
		/// Выполняем очистки буфера цвета:
		glClear(GL_COLOR_BUFFER_BIT);
		/// Выбираем для работы проэкционную матрицу:
		glMatrixMode(GL_PROJECTION);
		/// Устанавливем единичную(стандартную) проекционную матрицу:
		glLoadIdentity();
		/// Установим ортогональную проекцию:
		/// функцией void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
		/// То есть все трехмерное пространство будет ортогонально проецироваться
		/// на плоскость экрана в диапазоне координат (0,0,width,height);
		/// Для этого данная функция задает параллельный объем отсечения. 		
		/// Если рассматривать объем отсечения в декартовых координатах, 
		/// left и right будут минимальным и максимальным значениями х; 
		/// top и bottom — минимальным и максимальным значениями у, 
		/// a near и far — минимальным и максимальным значениями г.
		/// То есть такая проекция означает, что объекты удаленные от пользователя, не кажутся меньше
		/// (в перспективной проекции объекты уменьшаются)
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		/// Выбираем для работы модельно видовую матрицу:
		glMatrixMode(GL_MODELVIEW);
		/// Устанавливем единичную(стандартную) модельно видовую матрицу:
		/// По сути выполняем сброс всех ранее выполненных преобразования матрицы
		glLoadIdentity();
		/// Выполняем поворот пространства на угол glfwGetTime() * 50.f
		/// Где glfwGetTime() возвращает время в секундах с момента запуска приложения:
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		/// Рисуем треугольник по вершинам, для каждой вершины задаем произвольный цвет:
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		/// Выполняем свап буфера:
		glfwSwapBuffers(window);
		/// И обновления событий окна( в том числе клавиатуры и мыши):
		glfwPollEvents();
	}

	/// Если цикл не выполняется, значит окно закрывается и нужно его удалить из памяти:
	glfwDestroyWindow(window);
	/// Также нужно завершить работу с glfw, высвободить все ресурсы связанные с OpenGL, окном и прочее:
	glfwTerminate();
	/// Выйти из приложении сообщив о нормальном(штатном) завершении работы программы:
	exit(EXIT_SUCCESS);

}
