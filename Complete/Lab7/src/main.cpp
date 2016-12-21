#include "main.h"
#include "app.h"

int main() {
	App myApp;
	myApp.setup(1024, 768, "project4", true);

	/// ��������� ������� ���� ����������:
	/// ��������� ���� �� ��� ��� ���� ���� �� �����������:
	while (!glfwWindowShouldClose(myApp.getWindow())) {
		/// ��������� ���������� ������ ����������:
		myApp.update();
		/// ������ ����:
		myApp.draw();
		/// ��������� ���� ������:
		glfwSwapBuffers(myApp.getWindow());
		/// � ���������� ������� ����( � ��� ����� ���������� � ����):
		glfwPollEvents();
	}

	myApp.exit();

	cout << "main func()" << endl;
	system("pause");
}