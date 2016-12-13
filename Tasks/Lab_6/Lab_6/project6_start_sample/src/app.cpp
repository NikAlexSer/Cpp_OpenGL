#include "app.h"

/// Данные функции вызываются когда происходят события нажатия на клавиатуру, события с мышью и возмыжные неполадки:
 void App::error_callback(int error, const char* description){
	fputs(description, stderr);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

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



void App::mouse_callback(GLFWwindow* window, int a, int b, int c){
    cout << "button a = " << a  << endl;
    cout << "button b = " << b  << endl;
    cout << "button c = " << c  << endl;
}

/// Загрузка текстуры из bmp файла:
GLuint App::loadBmp(string fileName){
	/// Буфер в котором будет храниться путь:
	char buffer[MAX_PATH];
	/// Получаем путь до нашей программы которую мы запустили:
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	/// Конвертируем это в 
	string exePathWithName = string(buffer);
	/// Позиция последнего слеша:
	string::size_type pos  = exePathWithName.find_last_of( "\\/" );
	/// Копируем в переменную exePath исходные абсолютный путь до последнего слеша:
	string exePath		   = string(exePathWithName).substr( 0, pos);
	/// Формируем полный путь к изображению:
	string fullPath		   = exePath + "\\data\\" + fileName; 

	/// Открываем файл
	FILE * file = fopen(fullPath.c_str(),"rb");
	if (!file) {
		cout << "error load image!" << endl; 
		return 0;
	}

	unsigned char header[54];
	if (fread(header, 1, 54, file)!=54){ // У нас проблемы, если не смогли прочитать 54 байта		
		cout << "error read header!" << endl;
	}

	if(header[0] !='B' || header[1] !='M'){
		cout << "Not a correct BMP file" << endl;
		return 0;
	}

	/// Читаем int из массива байтов
	unsigned int dataPos    = *(int*)&(header[0x0A]);
	unsigned int imageSize  = *(int*)&(header[0x22]);
	unsigned int width      = *(int*)&(header[0x12]);
	unsigned int height     = *(int*)&(header[0x16]);



	/// В некоторых BMP файлах нет полной информации, поэтому мы её добавим сами:
	if (imageSize == 0)    imageSize = width * height * 3;	/// Один байт на каждую Red, Green, Blue компоненты
	if (dataPos == 0)      dataPos = 54;					/// Тут заканчивается заголовок, и по идее, должны начаться данные


	/// Создаем буфер:
	unsigned char * data = new unsigned char [imageSize];

	/// Читаем данные из файла в буфер:
	fread(data,1,imageSize,file);
	/// Теперь все данные в памяти, и можно закрыть файл:
	fclose(file);

	/// Создаем одну OpenGL текстуру:
	GLuint textureID;
	glGenTextures(1, &textureID);
	/// Биндим текстуру, и теперь все функции по работе с текстурами будут работать с этой текстурой:
	glBindTexture(GL_TEXTURE_2D, textureID);
	/// Отправляем картинку в OpenGL текстуру:
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	/// Указываем базовую фильтрацию:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	delete data;
	return textureID;
}



/// Загрузка сжатой текстуры с mip-map уровнями:
GLuint App::loadDDS(string fileName){
	char buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	string exePathWithName = string(buffer);
	string::size_type pos  = exePathWithName.find_last_of( "\\/" );
	string exePath		   = string(exePathWithName).substr( 0, pos);
	string fullPath		   = exePath + "\\data\\" + fileName; 

	/// Открываем файл
	FILE * file = fopen(fullPath.c_str(),"rb");
	if (!file) cout << "error load image!" << endl; 


	/// Проверяем чтобы разрешение было DDS:
	char filecode[4];
	fread(filecode, 1, 4, file);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(file);
		cout << "extension file wrong!" << endl;
		cout << "error load image!" << endl; 
		return 0;
	}
	
	/// Считываем параметры нашего DDS файала:
	unsigned char header[124];
	fread(&header, 124, 1, file);
	unsigned int height			= *(unsigned int*)&(header[8 ]);
	unsigned int width			= *(unsigned int*)&(header[12]);
	unsigned int linearSize     = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount	= *(unsigned int*)&(header[24]);
	unsigned int fourCC			= *(unsigned int*)&(header[80]);

	/// Читаем собственно само сжатое изображение, все его mip уровни:
	unsigned char * bufferData;
	unsigned int bufsize;
	/// Если число мип уровней больше одного - значит выделяем в 2 раза больше памяти под сами пиксельные данные:
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	bufferData = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	/// Читаем:
	fread(bufferData, 1, bufsize, file);
	/* закрываем указатель */
	fclose(file);


	/// Определяем используемый формат сжания доступный для OpenGL:
	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	/// Формат сжатия OpenGL:
	unsigned int format;

	/// Выбираем доступный формат OpenGL:	
	switch(fourCC){
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

	/// Создаем одну OpenGL текстуру
	GLuint textureID;
	glGenTextures(1, &textureID);
	/// Биндим текстуру, и теперь все функции по работе с текстурами будут работать с этой
	glBindTexture(GL_TEXTURE_2D, textureID);


	/// Передаем в OpenGL все mip уровни:
	unsigned int blockSize	= (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset		= 0;
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
	   unsigned int size = ((width + 3 )/4)*((height+3)/4)*blockSize;
	   glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, bufferData + offset);
	   offset += size;
	   width  /= 2;
	   height /= 2;
	}


	/// Когда картинка будет увеличиваться(нет большей Мипмапы), используем LINEAR фильтрацию
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/// Когда минимизируем — берем две ближних мипмапы и лиейно смешиваем цвета
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	/// И создаем сами мипмапы.
	glGenerateMipmap(GL_TEXTURE_2D);

	delete bufferData;
	return textureID;
}


/// Инициализируем геометрию и все что с этим может быть связано:
void App::setupScene(){
	/// Создаем VAO и выбираем его текущим:
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

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


	/// Конфигурируем буфер текстур:
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


	/// Применяем шейдер к геометрии:
	shader.setUniformMatrix4fv("MVP",&camera.getCameraMatrix()[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture1"), 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture2"), 1);

	glUniform1f(glGetUniformLocation(shader.getProgramID(), "mixFactor"), getInstance().mixFactor);
	
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

