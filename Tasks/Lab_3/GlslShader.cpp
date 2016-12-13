#include "GlslShader.h"

/// Получить путь к exe вместе с именем файла.
string GlslShader::exePathWithName() {
	char buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	return string( buffer );
}
/// Получить путь к exe вместе с без имени файла.
string GlslShader::exePath() {
	string buffer = exePathWithName();
	string::size_type pos = buffer.find_last_of( "\\/" );
	return string( buffer ).substr( 0, pos);
}

/// Основная функнция загрузки и компиляции вершинного и фрагментного шейдера и линковки в одну шейдерную программу GL:
void GlslShader::setup(string vertexShaderPath, string fragmentShaderPath){

    /// Создаем шейдеры:
    GLuint vertexShaderID	= glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    /// Читаем вершинный шейдер из файла:
    string vertexShaderCode;
	vertexShaderPath = exePath() + "\\" + vertexShaderPath;
    ifstream vertexShaderStream(vertexShaderPath.c_str(), std::ios::in);
    if(vertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(vertexShaderStream, Line))
            vertexShaderCode += "\n" + Line;
        vertexShaderStream.close();
    }

    /// Читаем фрагментный шейдер из файла:
    string fragmentShaderCode;
	fragmentShaderPath = exePath() + "\\" + fragmentShaderPath;
	ifstream fragmentShaderStream(fragmentShaderPath.c_str(), std::ios::in);
    if(fragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(fragmentShaderStream, Line))
            fragmentShaderCode += "\n" + Line;
        fragmentShaderStream.close();
    }


	GLint result = GL_FALSE;
    int infoLogLength;

	/// Компилируем вершинный шейдер:
	cout << "Compiling shader : " << vertexShaderPath << endl;
    char const * vertexSourcePointer = vertexShaderCode.c_str();
	/// Передаем код шейдера в OpenGL:
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	/// Собственно компиляция:
    glCompileShader(vertexShaderID);

	/// Устанавливаем параметры для возможности получения ошибок в текстовой консоли:
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    vector<char> vertexShaderErrorMessage(infoLogLength);
    glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

    /// Компилируем фрагментный шейдер:
	printf("Compiling shader : %s\n", fragmentShaderPath);
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	/// Передаем код шейдера в OpenGL:
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
    glCompileShader(fragmentShaderID);

	/// Устанавливаем параметры фрагментного шейдера для возможности получения ошибок в текстовой консоли:
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> fragmentShaderErrorMessage(infoLogLength);
    glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);

	/// Создаем шейдерную программу из фрагментного и вершинного шейдера:
	fprintf(stdout, "Linking program\n");
    programID = glCreateProgram();
	/// Линкуем шейдеры в программу:
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);	
    glLinkProgram(programID);

	/// Устанавливаем параметры для programID для возможности получения ошибок в текстовой консоли:
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::vector<char> programErrorMessage( max(infoLogLength, int(1)) );
    glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
    fprintf(stdout, "%s\n", &programErrorMessage[0]);

	/// Удаляем локальные фрагментные и вершинные шейдеры:
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

/// Вызвав эту функцию мы начинаем применять данный шейдер в отрисовке:
void GlslShader::use(){
	glUseProgram(programID);
}


/// Передаем в шейдер матрицу 4*4:
void GlslShader::setUniformMatrix4fv(string name, GLfloat* value){
	/// Начинаем использовать наш шейдер:
	GLuint uniformID = glGetUniformLocation(getProgramID(), name.c_str());
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, value);
}