#include "main.h"

int main(){
	App myApp;
	myApp.setup(1024,768,"project5",true);
	
	/// ��������� ������� ���� ����������:
	/// ��������� ���� �� ��� ��� ���� ���� �� �����������:
	while (!glfwWindowShouldClose(myApp.getWindow())){
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
}
