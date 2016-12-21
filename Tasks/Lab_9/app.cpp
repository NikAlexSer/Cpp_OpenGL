#include "app.h"

/// ������ ������� ���������� ����� ���������� ������� ������� �� ����������, ������� � ����� � ��������� ���������:
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

/// �������� �������� �� bmp �����:
GLuint App::loadBmp(string fileName){
	/// ����� � ������� ����� ��������� ����:
	char buffer[MAX_PATH];
	/// �������� ���� �� ����� ��������� ������� �� ���������:
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	/// ������������ ��� � 
	string exePathWithName = string(buffer);
	/// ������� ���������� �����:
	string::size_type pos  = exePathWithName.find_last_of( "\\/" );
	/// �������� � ���������� exePath �������� ���������� ���� �� ���������� �����:
	string exePath		   = string(exePathWithName).substr( 0, pos);
	/// ��������� ������ ���� � �����������:
	string fullPath		   = exePath + "\\data\\" + fileName; 

	/// ��������� ����
	FILE * file = fopen(fullPath.c_str(),"rb");
	if (!file) {
		cout << "error load image!" << endl; 
		return 0;
	}

	unsigned char header[54];
	if (fread(header, 1, 54, file)!=54){ // � ��� ��������, ���� �� ������ ��������� 54 �����		
		cout << "error read header!" << endl;
	}

	if(header[0] !='B' || header[1] !='M'){
		cout << "Not a correct BMP file" << endl;
		return 0;
	}

	/// ������ int �� ������� ������
	unsigned int dataPos    = *(int*)&(header[0x0A]);
	unsigned int imageSize  = *(int*)&(header[0x22]);
	unsigned int width      = *(int*)&(header[0x12]);
	unsigned int height     = *(int*)&(header[0x16]);



	/// � ��������� BMP ������ ��� ������ ����������, ������� �� � ������� ����:
	if (imageSize == 0)    imageSize = width * height * 3;	/// ���� ���� �� ������ Red, Green, Blue ����������
	if (dataPos == 0)      dataPos = 54;					/// ��� ������������� ���������, � �� ����, ������ �������� ������


	/// ������� �����:
	unsigned char * data = new unsigned char [imageSize];

	/// ������ ������ �� ����� � �����:
	fread(data,1,imageSize,file);
	/// ������ ��� ������ � ������, � ����� ������� ����:
	fclose(file);

	/// ������� ���� OpenGL ��������:
	GLuint textureID;
	glGenTextures(1, &textureID);
	/// ������ ��������, � ������ ��� ������� �� ������ � ���������� ����� �������� � ���� ���������:
	glBindTexture(GL_TEXTURE_2D, textureID);
	/// ���������� �������� � OpenGL ��������:
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	/// ��������� ������� ����������:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	delete data;
	return textureID;
}



/// �������� ������ �������� � mip-map ��������:
GLuint App::loadDDS(string fileName){
	char buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	string exePathWithName = string(buffer);
	string::size_type pos  = exePathWithName.find_last_of( "\\/" );
	string exePath		   = string(exePathWithName).substr( 0, pos);
	string fullPath		   = exePath + "\\data\\" + fileName; 

	/// ��������� ����
	FILE * file = fopen(fullPath.c_str(),"rb");
	if (!file) cout << "error load image!" << endl; 


	/// ��������� ����� ���������� ���� DDS:
	char filecode[4];
	fread(filecode, 1, 4, file);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(file);
		cout << "extension file wrong!" << endl;
		cout << "error load image!" << endl; 
		return 0;
	}
	
	/// ��������� ��������� ������ DDS ������:
	unsigned char header[124];
	fread(&header, 124, 1, file);
	unsigned int height			= *(unsigned int*)&(header[8 ]);
	unsigned int width			= *(unsigned int*)&(header[12]);
	unsigned int linearSize     = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount	= *(unsigned int*)&(header[24]);
	unsigned int fourCC			= *(unsigned int*)&(header[80]);

	/// ������ ���������� ���� ������ �����������, ��� ��� mip ������:
	unsigned char * bufferData;
	unsigned int bufsize;
	/// ���� ����� ��� ������� ������ ������ - ������ �������� � 2 ���� ������ ������ ��� ���� ���������� ������:
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	bufferData = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	/// ������:
	fread(bufferData, 1, bufsize, file);
	/* ��������� ��������� */
	fclose(file);


	/// ���������� ������������ ������ ������ ��������� ��� OpenGL:
	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	/// ������ ������ OpenGL:
	unsigned int format;

	/// �������� ��������� ������ OpenGL:	
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

	/// ������� ���� OpenGL ��������
	GLuint textureID;
	glGenTextures(1, &textureID);
	/// ������ ��������, � ������ ��� ������� �� ������ � ���������� ����� �������� � ����
	glBindTexture(GL_TEXTURE_2D, textureID);


	/// �������� � OpenGL ��� mip ������:
	unsigned int blockSize	= (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset		= 0;
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
	   unsigned int size = ((width + 3 )/4)*((height+3)/4)*blockSize;
	   glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, bufferData + offset);
	   offset += size;
	   width  /= 2;
	   height /= 2;
	}


	/// ����� �������� ����� �������������(��� ������� �������), ���������� LINEAR ����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/// ����� ������������ � ����� ��� ������� ������� � ������ ��������� �����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	/// � ������� ���� �������.
	glGenerateMipmap(GL_TEXTURE_2D);

	delete bufferData;
	return textureID;
}


/// �������������� ����� � ��� ��� � ���� ����� ���� �������:
void App::setupScene(){

	/// �������������� ������:
	shader.setup("data\\simpleShader.vert","data\\simpleShader.frag");

	/// �������������� ����� ��������� skyBox, ������ ������ ����� ��������� ��������, � ����� ��� ������:
	skyBox.setup("skybox",500);

	float groundY = -1.0;

	/// �������������� ������ ����������� ��������� �� �����:
	shader3Dmodel.setup("data\\PhongSimpleLight.vert","data\\PhongSimpleLight.frag");

	/// �������������� ������ ����������� ��������� �� ����� � �������������� ����� ������:
	shader3Dmodel2.setup("data\\PhongSimpleLight2.vert","data\\PhongSimpleLight2.frag");

	shader3Dmodel3.setup("data\\PhongSimpleLight3.vert","data\\PhongSimpleLight3.frag");


	lightPoint.setup(window,1,1,1);
	lightPoint.setPosition(-3,-0.5,0);

	ground.setup("Plane001");
	ground.setPosition(-5,groundY-0.05,0);	
	ground.setRotate(-90,vec3(1,0,0));
	ground.setScale(0.1,0.1,0.1);
	ground.setShader(&shader3Dmodel);
	ground.setLight(&lightPoint);

	house.setup("house",true, true, true);
	house.setPosition(-6,groundY,1);	
	house.setScale(0.03,0.03,0.03);
	house.setRotate(-90,vec3(1,0,0));
	house.setRotate(180,vec3(0,0,1));
	house.setShader(&shader3Dmodel3);
	house.setLight(&lightPoint);
	house.setSpecularStrength(2);

	teapot.setup("Teapot001");
	teapot.setPosition(-4.2,groundY,5.2);		
	teapot.setRotate(-90,vec3(1,0,0));
	teapot.setScale(0.02,0.02,0.02);
	teapot.setShader(&shader3Dmodel);
	teapot.setLight(&lightPoint);


	brickWall.setup("Plane002",true, false,true);
	brickWall.setPosition(-1,0,0);	
	brickWall.setRotate(0,vec3(1,0,0));
	brickWall.setScale(0.012,0.012,0.012);

	brickWall.setShader(&shader3Dmodel3);
	brickWall.setLight(&lightPoint);
	brickWall.setSpecularStrength(1.0);

}

void App::createTextureCube(){
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	/// ������ �� 3 �������� ������� ����� ������������ 3 �������:
	static const GLfloat gVertexBufferData[] = {

		/// 1 �����:
	   -1.0f,-1.0f,-1.0f, 
	   -1.0f,-1.0f, 1.0f,
	   -1.0f, 1.0f, 1.0f, 

	   -1.0f,-1.0f,-1.0f,
	   -1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f,-1.0f,

	   /// 2 �����:
	   1.0f, 1.0f,-1.0f,
	   -1.0f,-1.0f,-1.0f,
	   -1.0f, 1.0f,-1.0f, 
	   	1.0f, 1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	   -1.0f,-1.0f,-1.0f,

	   /// 3 �����:
	   1.0f,-1.0f, 1.0f,
	   -1.0f,-1.0f,-1.0f,
	   1.0f,-1.0f,-1.0f,
	    1.0f,-1.0f, 1.0f,
	   -1.0f,-1.0f, 1.0f,
	   -1.0f,-1.0f,-1.0f,
	   	   
	   /// 4 �����:
	    1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,	   
	   	1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f,-1.0f,
	   1.0f, 1.0f,-1.0f,

	    /// 5 �����:
	   -1.0f, 1.0f, 1.0f,
	   -1.0f,-1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,
	   	1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f, 1.0f,
	   1.0f,-1.0f, 1.0f,

	   /// 6 �����:
	   1.0f, 1.0f, 1.0f,
	   1.0f, 1.0f,-1.0f,
	   -1.0f, 1.0f,-1.0f,
	    1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f,-1.0f,
	   -1.0f, 1.0f, 1.0f,	 
	};

	/// ������� ���������� OpenGL ����� � ��������� ��������� �� ���� � vertexBuffer:
	glGenBuffers(1, &vertexBuffer);
	/// �������� ������� vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// ������������� ���� ������� � OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	/// ��������� ��������, � ���������� � ������������� � texture1:
	texture1 = loadBmp("container.bmp");
	texture1 = loadDDS("container.dds");
	texture2 = loadDDS("iron.dds");
	/// ��������� ����������:
	mixFactor = 0;

	/// �� ������ ������� �� ��� UV ����������:
	static const GLfloat gUVBufferData[] = {

	   /// ��������������� ���������� ��� 1 ����� ����:
	   1.0f, 0.0f,
	   0.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 0.0f,
	   0.0f, 1.0f,
	   1.0f, 1.0f,

	   /// ��������������� ���������� ��� 2 ����� ����:
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

/// ��������� �������������:
void App::setup(int w, int h, string captionWindow, bool useAntiAliasing){
	/// ���������� ������� error_callback ��� ������������ ��������� ������ ��������� � GL ���
	/// ��������� ����������� GLFW
	glfwSetErrorCallback(error_callback);

	/// ��������� ������������� ���������� GLFW:
	if (!glfwInit()) std::exit(EXIT_FAILURE);
	/// �������� 4 ������� ����������� ��� ���������� ������� ������������� �����������:
	if (useAntiAliasing) glfwWindowHint(GLFW_SAMPLES,4);

	/// ������� GLFW ���� � ������ �������� � ����������, ���������� ��������� �� ������� ������ ��� 
	/// ��� ��������� ������ ����:
	window = glfwCreateWindow(w, h, captionWindow.c_str(), NULL, NULL);
	
	/// ���� ���� �� �������, �� ���� ��������� �������� NULL - �������� ��������� ���� ����������:
    if (!window){
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

	/// ���������� ��������� �������� OpenGL ��� ���� window:
    glfwMakeContextCurrent(window);
	/// ������ ���������� ��������  ���������� ������:
    glfwSwapInterval(1);
	/// ���������� ������������ ������� ���� � ����������:
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

	/// �������������� GLEW:
	glewInit();
	/// �������������� ��������� � ��� ��� � ���� ����� ���� �������:
	setupScene();

	/// ������ ��������� ������:
	camera.setup(getWindow(),vec3(0,0,5),3.14,0.0f,45.f,3,0.01,1000);	
}


/// ��������� ���������� ���� C++ ���������� - ���� ������ ����������:
void App::update(){

	/// ��������� ������:
	camera.update();
	/// ��������� skyBox:
	skyBox.update(camera.getCameraMatrix(), camera.getPosition());

	lightPoint.update();

	if (glfwGetKey(window, GLFW_KEY_1)	== GLFW_PRESS)	house.enableSpecular(true);
	if (glfwGetKey(window, GLFW_KEY_2)	== GLFW_PRESS)	house.enableSpecular(false);


	//house.update(camera.getCameraMatrix());
	//ground.update(camera.getCameraMatrix());
	//teapot.update(camera.getCameraMatrix());

	house.update  (camera.getCameraModelMatrix(),	camera.getCameraViewMatrix(),	camera.getCameraProjectionMatrix());
	house.setCameraPos(camera.getPosition());
	ground.update (camera.getCameraModelMatrix(),	camera.getCameraViewMatrix(),	camera.getCameraProjectionMatrix());
	teapot.update (camera.getCameraModelMatrix(),	camera.getCameraViewMatrix(),	camera.getCameraProjectionMatrix());


	if (glfwGetKey(window, GLFW_KEY_3)	== GLFW_PRESS)	{
		brickWall.setShader(&shader3Dmodel);
		house.setShader(&shader3Dmodel2);
	}
	if (glfwGetKey(window, GLFW_KEY_4)	== GLFW_PRESS)	{
		brickWall.setShader(&shader3Dmodel3);
		house.setShader(&shader3Dmodel3);
	}

	brickWall.update  (camera.getCameraModelMatrix(),	camera.getCameraViewMatrix(),	camera.getCameraProjectionMatrix());
	brickWall.setCameraPos(camera.getPosition());

}

void App::drawTextureCube(){	

	shader.use();
	/// �������� ������� ������:
	shader.setUniformMatrix4fv("MVP",&camera.getCameraMatrix()[0][0]);
	/// �������� ����� ����:
	glBindVertexArray(cubeVAO);
	/// �������� 0 ������� ������:
	glEnableVertexAttribArray(0);
	/// ������ ��� ���� �������� ����� � ������������, � �������� ��� � �������� ��������:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // ������� 0. ����������� ������ � ���� ���, �� ����� ������ ��������� � ������ � �������
		3,                  // ����������
		GL_FLOAT,           // ���
		GL_FALSE,           // ��������������� ��?
		0,                  // ���
		(void*)0            // �������� � ������
	);

	/// ���� �����, ������ ��� ���������� ���������:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // �������. ��� ������ ������ ������ 1, �������, ����� ��������� �� ��������� � �������.
		2,                                // ������
		GL_FLOAT,                         // ���
		GL_FALSE,                         // ��������������� ��?
		0,                                // ���
		(void*)0                          //�������� � ������
	);

	/// 1 ��������:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture1"), 0);	
	/////// 2 ��������:
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture2"), 1);
	shader.setUniform1f("mixFactor",getInstance().mixFactor);
	/// ������ �������������� ���:
	glDrawArrays(GL_TRIANGLES, 0, 36); 
}

/// ��������� ��������� ������ �����:
void App::draw(){
	/// ��������� ������� ������ �����:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.5f,0.0);
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS); 
		ground.draw(true);
		house.draw(true);
		teapot.draw(true);
		brickWall.draw(true);
		lightPoint.draw(camera.getCameraMatrix());
	glBindVertexArray(0);

	/// ������ ������� ���������:
	skyBox.draw();
}

/// ����� �� ����������:
void App::exit(){
	/// ���� ���� �� �����������, ������ ���� ����������� � ����� ��� ������� �� ������:
    glfwDestroyWindow(window);
	/// ����� ����� ��������� ������ � glfw, ����������� ��� ������� ��������� � OpenGL, ����� � ������:
    glfwTerminate();
	/// ����� �� ���������� ������� � ����������(�������) ���������� ������ ���������:
    std::exit(EXIT_SUCCESS);
}

