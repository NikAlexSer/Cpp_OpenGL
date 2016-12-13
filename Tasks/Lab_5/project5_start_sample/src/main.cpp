#include "main.h"

int main(){
	App myApp;
	myApp.setup(1024,768,"project4",true);
	
	/// Запускаем главный цикл обновления:
	/// Выполняем цикл до тех пор пока окно не закрывается:
	while (!glfwWindowShouldClose(myApp.getWindow())){
		/// Выполняем обновление нашего приложения:
		myApp.update();
		/// Рисуем кадр:
		myApp.draw();
		/// Выполняем свап буфера:
        glfwSwapBuffers(myApp.getWindow());
		/// И обновления событий окна( в том числе клавиатуры и мыши):
        glfwPollEvents();		
    }
	myApp.exit();
}
