#include "GlslShader.h"

/// �������� ���� � exe ������ � ������ �����.
string GlslShader::exePathWithName() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return string(buffer);
}
/// �������� ���� � exe ������ � ��� ����� �����.
string GlslShader::exePath() {
	string buffer = exePathWithName();
	string::size_type pos = buffer.find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

/// �������� �������� �������� � ���������� ���������� � ������������ ������� � �������� � ���� ��������� ��������� GL:
void GlslShader::setup(string vertexShaderPath, string fragmentShaderPath) {

	/// ������� �������:
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	/// ������ ��������� ������ �� �����:
	string vertexShaderCode;
	vertexShaderPath = exePath() + "\\" + vertexShaderPath;
	ifstream vertexShaderStream(vertexShaderPath.c_str(), std::ios::in);
	if (vertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(vertexShaderStream, Line))
			vertexShaderCode += "\n" + Line;
		vertexShaderStream.close();
	}

	/// ������ ����������� ������ �� �����:
	string fragmentShaderCode;
	fragmentShaderPath = exePath() + "\\" + fragmentShaderPath;
	ifstream fragmentShaderStream(fragmentShaderPath.c_str(), std::ios::in);
	if (fragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(fragmentShaderStream, Line))
			fragmentShaderCode += "\n" + Line;
		fragmentShaderStream.close();
	}


	GLint result = GL_FALSE;
	int infoLogLength;

	/// ����������� ��������� ������:
	cout << "Compiling shader : " << vertexShaderPath << endl;
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	/// �������� ��� ������� � OpenGL:
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	/// ���������� ����������:
	glCompileShader(vertexShaderID);

	/// ������������� ��������� ��� ����������� ��������� ������ � ��������� �������:
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	vector<char> vertexShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

	/// ����������� ����������� ������:
	printf("Compiling shader : %s\n", fragmentShaderPath);
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	/// �������� ��� ������� � OpenGL:
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	/// ������������� ��������� ������������ ������� ��� ����������� ��������� ������ � ��������� �������:
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> fragmentShaderErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);

	/// ������� ��������� ��������� �� ������������ � ���������� �������:
	fprintf(stdout, "Linking program\n");
	programID = glCreateProgram();
	/// ������� ������� � ���������:
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	/// ������������� ��������� ��� programID ��� ����������� ��������� ������ � ��������� �������:
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> programErrorMessage(max(infoLogLength, int(1)));
	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
	fprintf(stdout, "%s\n", &programErrorMessage[0]);

	/// ������� ��������� ����������� � ��������� �������:
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

/// ������ ��� ������� �� �������� ��������� ������ ������ � ���������:
void GlslShader::use() {
	glUseProgram(programID);
}


/// �������� � ������ ������� 4*4:
void GlslShader::setUniformMatrix4fv(string name, GLfloat* value) {
	/// �������� ������������ ��� ������:
	GLuint uniformID = glGetUniformLocation(getProgramID(), name.c_str());
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, value);
}