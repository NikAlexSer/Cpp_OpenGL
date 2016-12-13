/// ���������� ���� app.h ����� app.cpp ����� ���������� ������� ����� ���������� GLFW:
#include "app.h"

/// ������ ������� ���������� ����� ���������� ������� ������� �� ����������, ������� � ����� � ��������� ���������:
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

/// ��� �������� ������� main() ������� �������� ������ ����� � ���� ����������, ������ �� ��� ��������� ��� ������ ���������� ������ 
/// ����������:
int main() {
	/// ��������� �� ������������ ����:
	GLFWwindow* window;
	/// ���������� ������� error_callback ��� ������������ ��������� ������ ��������� � GL ���
	/// ��������� ����������� GLFW
	glfwSetErrorCallback(error_callback);
	/// ��������� ������������� ���������� GLFW:
	if (!glfwInit())
		exit(EXIT_FAILURE);
	/// �������� 4 ������� ����������� ��� ���������� ������� ������������� �����������:
	glfwWindowHint(GLFW_SAMPLES, 4);

	/// �������� ����������� ������ ������ OpenGL 3.3:
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// ������� GLFW ���� � ������ �������� � ����������, ���������� ��������� �� ������� ������ ��� 
	/// ��� ��������� ������ ����:
	window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);

	/// ���� ���� �� �������, �� ���� ��������� �������� NULL - �������� ��������� ���� ����������:
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/// ���������� ��������� �������� OpenGL ��� ���� window:
	glfwMakeContextCurrent(window);
	/// ������ ���������� ��������  ���������� ������:
	glfwSwapInterval(1);
	/// ���������� ������������ ������� ���� � ����������:
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);












	glewInit();
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/// ������ �� 3 �������� ������� ����� ������������ 3 �������
	static const GLfloat gVertexBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	/// ������������� ���������� ������:
	GLuint vertexBuffer;
	/// ������� ���������� OpenGL ����� � ��������� ��������� �� ���� � vertexBuffer:
	glGenBuffers(1, &vertexBuffer);
	/// �������� ������� vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// ������������� ���� ������� � OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	/// ��������� ������:
	GlslShader shader;
	shader.setup("simpleShader.vert", "simpleShader.frag");


	/// ��������� ���� ���������� �������� � ����:
	/// ��������� ���� �� ��� ��� ���� ���� �� �����������:
	while (!glfwWindowShouldClose(window))
	{
		/*
		/// ��������� ����������� ������ ����:
		float ratio;
		/// ������ � ������ ����:
		int width, height;
		/// � �������� ������� �������� ������ ����:
		glfwGetFramebufferSize(window, &width, &height);
		/// �������� ������� �������:
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		/// ��������� ��������� ����������� ������:
		ratio = width / (float)height;
		/// ����� ���� ������� GL �������:
		/// ������ ��������� ������� ������ ����������� �� ������:
		/// �� ���� �� ������� ��� ����� ������������ ����������� OpenGL:
		glViewport(0, 0, width, height);
		/// ��������� ������� ������ �����:
		glClear(GL_COLOR_BUFFER_BIT);
		/// �������� ��� ������ ������������ �������:
		glMatrixMode(GL_PROJECTION);
		/// ������������ ���������(�����������) ������������ �������:
		glLoadIdentity();
		/// ��������� ������������� ��������:
		/// �������� void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
		/// �� ���� ��� ���������� ������������ ����� ������������ ��������������
		/// �� ��������� ������ � ��������� ��������� (0,0,width,height);
		/// ��� ����� ������ ������� ������ ������������ ����� ���������.
		/// ���� ������������� ����� ��������� � ���������� �����������,
		/// left � right ����� ����������� � ������������ ���������� �;
		/// top � bottom � ����������� � ������������ ���������� �,
		/// a near � far � ����������� � ������������ ���������� �.
		/// �� ���� ����� �������� ��������, ��� ������� ��������� �� ������������, �� ������� ������
		/// (� ������������� �������� ������� �����������)
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		/// �������� ��� ������ �������� ������� �������:
		glMatrixMode(GL_MODELVIEW);
		/// ������������ ���������(�����������) �������� ������� �������:
		/// �� ���� ��������� ����� ���� ����� ����������� �������������� �������
		glLoadIdentity();
		/// ��������� ������� ������������ �� ���� glfwGetTime() * 50.f
		/// ��� glfwGetTime() ���������� ����� � �������� � ������� ������� ����������:
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		/// ������ ����������� �� ��������, ��� ������ ������� ������ ������������ ����:
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		/// ��������� ���� ������:
		glfwSwapBuffers(window);
		/// � ���������� ������� ����( � ��� ����� ���������� � ����):
		glfwPollEvents();
	}
	*/
	/// ��������� ������� ������ �����:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.5f, 0.0);

		/// �������� ������������ ��� ������:
		shader.use();

	/// ������ ����� ���������: �������
		glEnableVertexAttribArray(0);
		/// �������� ������� ��������� �����:
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // ������� 0. ����������� ������ � ���� ���, �� ����� ������ ��������� � ������ � �������
			3,                  // ����������
			GL_FLOAT,           // ���
			GL_FALSE,           // ��������������� ��?
			0,                  // ���
			(void*)0            // �������� � ������
		);

		/// ������ ����������� !
		glDrawArrays(GL_TRIANGLES, 0, 3); //������� � ������� 0 � ������ 3 �����. ����� => 1 ����������� 
		glDisableVertexAttribArray(0);

		/// ��������� ���� ������:
		glfwSwapBuffers(window);
		/// � ���������� ������� ����( � ��� ����� ���������� � ����):
		glfwPollEvents();
	}
	/// ���� ���� �� �����������, ������ ���� ����������� � ����� ��� ������� �� ������:
	glfwDestroyWindow(window);
	/// ����� ����� ��������� ������ � glfw, ����������� ��� ������� ��������� � OpenGL, ����� � ������:
	glfwTerminate();
	/// ����� �� ���������� ������� � ����������(�������) ���������� ������ ���������:
	exit(EXIT_SUCCESS);
}
