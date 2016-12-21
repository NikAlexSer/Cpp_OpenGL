#include "SimpleLoaderCollada3D.h"

/// Вспомогательные функции для парсинга строки:
void SimpleLoaderCollada3D::split(const std::string &s, char delim, std::vector<string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}
std::vector<std::string> SimpleLoaderCollada3D::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void SimpleLoaderCollada3D::draw(){
		shader.use();
		/// Передаем матрицу камеры:
		shader.setUniformMatrix4fv("modelMatrix",&modelMatrix[0][0]);
		shader.setUniformMatrix4fv("MVP",&globalMVP[0][0]);
        glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
		glUniform1i(glGetUniformLocation(shader.getProgramID(), "diffuseTexture"), 0);	

		glDrawArrays(GL_TRIANGLES, 0, outVertices.size()); 
        //glDrawElements(GL_TRIANGLES, outVertices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
}

void SimpleLoaderCollada3D::update(mat4 MVP){
	globalMVP = MVP;
}

/// Получить массив 3D/2D координат из тега содержащий числа разделенные пробелами:
template<typename T> 
void SimpleLoaderCollada3D::getPointsArrayFromStringTag(TiXmlElement* tag, vector <T>& pointsOut){
	if (tag){
		/// Узнаем количество чисел, по 3 числа на кажду вершину:
		int numElements = std::stoi(tag->Attribute("count"));
		/// Получаем содержимое тега "float_array":
		string values = tag->GetText();
		/// Обнуляем вектор-массив вершин:
		pointsOut.clear();
		/// Получаем массив из строки, где каждый элемент кусок строки до следующего пробела
		/// Таким образом мы получим массив элементов где каждый элемент - это число:
		vector<string> valuesArrays = split(values,' ');
		/// Вычисляем шаг приращения, определяя сколько float чисел содержится во входном типе данных:
		int stepSize = sizeof(T) / 4;	
		/// Записываем числа в виде трехмерных или двумерных координат в зависимости от pointsOut:
		for (int i = 0; i < numElements; i+=stepSize){
			/// Всегда используем vec3, если входной тип данных(T есть vec2) двумерный то выполним приведение
			/// при котором 3 координата point отбрасывается:
			vec3 point;
			point.x = std::stof(valuesArrays[i]);
			point.y = std::stof(valuesArrays[i+1]);
			/// Проверка нужно чтобы не выйти за пределы массива если в теге содержатся двумерные значения:
			if (stepSize == 3) point.z = std::stof(valuesArrays[i+2]);		
			pointsOut.push_back((T)point);
		}
	}
}

bool SimpleLoaderCollada3D::setup(string folder){
	/// Получаем корректный абсолютный путь до файла DAE:
	char buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	string exePathWithName = string(buffer);
	string::size_type pos  = exePathWithName.find_last_of( "\\/" );
	string exePath		   = string(exePathWithName).substr( 0, pos);
	string fullPath		   = exePath + "\\data\\" + folder + "\\mesh.DAE"; 

	/// Пытаемся прочитать этот файл:
	TiXmlDocument doc(fullPath.c_str());	
	doc.LoadFile();
	/// 
	if (doc.Error()){
		printf( "Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );	
		return false;
	}

	/// Находим тег "COLLADA"
	TiXmlElement* collada = doc.FirstChildElement("COLLADA");
	/// Если он есть:
	if (collada){		
		/// Находим тег "library_geometries"
		TiXmlElement* libraryGeometries = collada->FirstChildElement("library_geometries");
		/// Если он есть:
		if (libraryGeometries){
			/// Находим тег "geometry"
			TiXmlElement* geometry = libraryGeometries->FirstChildElement("geometry");
			/// Если он есть:
			if (geometry) {
				TiXmlElement* mesh = geometry->FirstChildElement("mesh");
				if(mesh){
					/// Получаем первый источник информации о модели - координаты вершин:
					TiXmlElement* source = mesh->FirstChildElement("source");
					if(source){
						/// Получаем доступ к координатам вершин нашей модели:
						/// Проверка по атрибуту id:
						if (source->Attribute("id") == folder + "-POSITION") {
							TiXmlElement* floatArray = source->FirstChildElement("float_array");
							getPointsArrayFromStringTag(floatArray,coords);													
						}
					}

					/// Загружаем данные о нормалях вершин:
					source =  source->NextSiblingElement();
					if (source){
						/// Проверка по атрибуту id:
						if (source->Attribute("id") == folder + "-Normal0") {
							TiXmlElement* floatArray = source->FirstChildElement("float_array");
							getPointsArrayFromStringTag(floatArray,normals);		
						}
					}

					/// Загружаем данные о нормалях вершин:
					source =  source->NextSiblingElement();
					if (source){				
						/// Проверка по атрибуту id:
						if (source->Attribute("id") == folder + "-UV0") {
							TiXmlElement* floatArray = source->FirstChildElement("float_array");
							getPointsArrayFromStringTag(floatArray,uvCoords);		
						}
					}
		
					/// После того как загрузили координаты вершин, нормалей и текстуры начинаем загружать 
					/// индексы вершин, нормалей и текстур которые будут описывать из каких конкретной вершин и нормалей будут формироваться 
					/// треугольники 3D модели:
					TiXmlElement* triangles = mesh->FirstChildElement("triangles");
					if (triangles){
						TiXmlElement* p = triangles->FirstChildElement("p");
						if (p){
							/// Узнаем сколько треугольников в модели:
							int numElements = std::stoi(triangles->Attribute("count"));
							/// Получаем содержимое тега "float_array":
							string values = p->GetText();
							/// Получаем массив из строки, где каждый элемент кусок строки до следующего пробела
							/// Таким образом мы получим массив элементов где каждый элемент - это число:
							vector<string> valuesArrays = split(values,' ');
						
							outVertices.clear();
							for (int i = 0; i < numElements; i++) {
								/// 1 Вершина треугольника:
								Vertex p1;
								/// Индекс позиции координаты для 1 вершины треугольника:
								GLuint posIndex1 = std::stoi(valuesArrays[0+(i*9)]);
								p1.position = coords[posIndex1];
								/// Индекс позиции координаты нормали для 1 вершины треугольника:
								GLuint normalIndex1 = std::stoi(valuesArrays[1+(i*9)]);
								p1.normal = normals[normalIndex1];
								/// Индекс позиции текстурной координаты для 1 вершины треугольника:
								GLuint uvIndex1 = std::stoi(valuesArrays[2+(i*9)]);
								p1.texCoords = uvCoords[uvIndex1];
								
								/// 2 Вершина треугольника:
								Vertex p2;
								/// Индекс позиции координаты для 2 вершины треугольника:
								GLuint posIndex2 = std::stoi(valuesArrays[3+(i*9)]);
								p2.position = coords[posIndex2];
								/// Индекс позиции координаты нормали для 2 вершины треугольника:
								GLuint normalIndex2 = std::stoi(valuesArrays[4+(i*9)]);
								p2.normal = normals[normalIndex2];
								/// Индекс позиции текстурной координаты для 2 вершины треугольника:
								GLuint uvIndex2 = std::stoi(valuesArrays[5+(i*9)]);
								p2.texCoords = uvCoords[uvIndex2];
								
								/// 3 Вершина треугольника:
								Vertex p3;
								/// Индекс позиции координаты для 2 вершины треугольника:
								GLuint posIndex3 = std::stoi(valuesArrays[6+(i*9)]);
								p3.position = coords[posIndex3];
								/// Индекс позиции координаты нормали для 2 вершины треугольника:
								GLuint normalIndex3 = std::stoi(valuesArrays[7+(i*9)]);
								p3.normal = normals[normalIndex3];
								/// Индекс позиции текстурной координаты для 2 вершины треугольника:
								GLuint uvIndex3 = std::stoi(valuesArrays[8+(i*9)]);
								p3.texCoords = uvCoords[uvIndex3];

								/// Формируем треугольник из 3 вершин:
								outVertices.push_back(p1);
								outVertices.push_back(p2);
								outVertices.push_back(p3);
							}
						}
					}

					/// Традиционно - инициализируем буферы OpenGL и передаем прочитанные данные в OpenGL:
					glGenVertexArrays(1, &VAO);
					glGenBuffers(1, &VBO);

					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					/// Предоставляем наши вершины в OpenGL:
					glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(Vertex), &outVertices[0], GL_STATIC_DRAW);

					/// Выставляем атрибуты координат вершин:
					glEnableVertexAttribArray(0);	
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
					/// Выставляем атрибуты нормали:
					glEnableVertexAttribArray(1);	
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
					/// Выставляем атрибуты текстурных координат:
					glEnableVertexAttribArray(2);	
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
					glBindVertexArray(0);					
				}
			}
		}
	}

	/// Инициализируем шейдер:
	shader.setup("data\\simpleShader2.vert","data\\simpleShader2.frag");
	loaderImage loader;
	diffuseTexture = loader.loadDDS(folder + "\\diffuse_map.dds");
	/// Инициализируем модельную матрицу единичной:
	modelMatrix = mat4(1.0f);

	return true;
}

void SimpleLoaderCollada3D::setScale(float x, float y, float z){
	modelMatrix = glm::scale(modelMatrix,vec3(x,y,z));
}

void SimpleLoaderCollada3D::setPosition(float x, float y, float z){
	modelMatrix = glm::translate(modelMatrix,vec3(x,y,z));
}

void SimpleLoaderCollada3D::setRotate(float angle, vec3 axis){
	float rad = (angle * 3.14159265359 / 180.0);
	modelMatrix = glm::rotate(modelMatrix,rad,axis);
}

