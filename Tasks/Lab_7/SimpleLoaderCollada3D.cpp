#include "SimpleLoaderCollada3D.h"

/// ��������������� ������� ��� �������� ������:
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
		/// �������� ������� ������:
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

/// �������� ������ 3D/2D ��������� �� ���� ���������� ����� ����������� ���������:
template<typename T> 
void SimpleLoaderCollada3D::getPointsArrayFromStringTag(TiXmlElement* tag, vector <T>& pointsOut){
	if (tag){
		/// ������ ���������� �����, �� 3 ����� �� ����� �������:
		int numElements = std::stoi(tag->Attribute("count"));
		/// �������� ���������� ���� "float_array":
		string values = tag->GetText();
		/// �������� ������-������ ������:
		pointsOut.clear();
		/// �������� ������ �� ������, ��� ������ ������� ����� ������ �� ���������� �������
		/// ����� ������� �� ������� ������ ��������� ��� ������ ������� - ��� �����:
		vector<string> valuesArrays = split(values,' ');
		/// ��������� ��� ����������, ��������� ������� float ����� ���������� �� ������� ���� ������:
		int stepSize = sizeof(T) / 4;	
		/// ���������� ����� � ���� ���������� ��� ��������� ��������� � ����������� �� pointsOut:
		for (int i = 0; i < numElements; i+=stepSize){
			/// ������ ���������� vec3, ���� ������� ��� ������(T ���� vec2) ��������� �� �������� ����������
			/// ��� ������� 3 ���������� point �������������:
			vec3 point;
			point.x = std::stof(valuesArrays[i]);
			point.y = std::stof(valuesArrays[i+1]);
			/// �������� ����� ����� �� ����� �� ������� ������� ���� � ���� ���������� ��������� ��������:
			if (stepSize == 3) point.z = std::stof(valuesArrays[i+2]);		
			pointsOut.push_back((T)point);
		}
	}
}

bool SimpleLoaderCollada3D::setup(string folder){
	/// �������� ���������� ���������� ���� �� ����� DAE:
	char buffer[MAX_PATH];
	GetModuleFileNameA( NULL, buffer, MAX_PATH );  
	string exePathWithName = string(buffer);
	string::size_type pos  = exePathWithName.find_last_of( "\\/" );
	string exePath		   = string(exePathWithName).substr( 0, pos);
	string fullPath		   = exePath + "\\data\\" + folder + "\\mesh.DAE"; 

	/// �������� ��������� ���� ����:
	TiXmlDocument doc(fullPath.c_str());	
	doc.LoadFile();
	/// 
	if (doc.Error()){
		printf( "Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );	
		return false;
	}

	/// ������� ��� "COLLADA"
	TiXmlElement* collada = doc.FirstChildElement("COLLADA");
	/// ���� �� ����:
	if (collada){		
		/// ������� ��� "library_geometries"
		TiXmlElement* libraryGeometries = collada->FirstChildElement("library_geometries");
		/// ���� �� ����:
		if (libraryGeometries){
			/// ������� ��� "geometry"
			TiXmlElement* geometry = libraryGeometries->FirstChildElement("geometry");
			/// ���� �� ����:
			if (geometry) {
				TiXmlElement* mesh = geometry->FirstChildElement("mesh");
				if(mesh){
					/// �������� ������ �������� ���������� � ������ - ���������� ������:
					TiXmlElement* source = mesh->FirstChildElement("source");
					if(source){
						/// �������� ������ � ����������� ������ ����� ������:
						/// �������� �� �������� id:
						if (source->Attribute("id") == folder + "-POSITION") {
							TiXmlElement* floatArray = source->FirstChildElement("float_array");
							getPointsArrayFromStringTag(floatArray,coords);													
						}
					}

					/// ��������� ������ � �������� ������:
					source =  source->NextSiblingElement();
					if (source){
						/// �������� �� �������� id:
						if (source->Attribute("id") == folder + "-Normal0") {
							TiXmlElement* floatArray = source->FirstChildElement("float_array");
							getPointsArrayFromStringTag(floatArray,normals);		
						}
					}

					/// ��������� ������ � �������� ������:
					source =  source->NextSiblingElement();
					if (source){				
						/// �������� �� �������� id:
						if (source->Attribute("id") == folder + "-UV0") {
							TiXmlElement* floatArray = source->FirstChildElement("float_array");
							getPointsArrayFromStringTag(floatArray,uvCoords);		
						}
					}
		
					/// ����� ���� ��� ��������� ���������� ������, �������� � �������� �������� ��������� 
					/// ������� ������, �������� � ������� ������� ����� ��������� �� ����� ���������� ������ � �������� ����� ������������� 
					/// ������������ 3D ������:
					TiXmlElement* triangles = mesh->FirstChildElement("triangles");
					if (triangles){
						TiXmlElement* p = triangles->FirstChildElement("p");
						if (p){
							/// ������ ������� ������������� � ������:
							int numElements = std::stoi(triangles->Attribute("count"));
							/// �������� ���������� ���� "float_array":
							string values = p->GetText();
							/// �������� ������ �� ������, ��� ������ ������� ����� ������ �� ���������� �������
							/// ����� ������� �� ������� ������ ��������� ��� ������ ������� - ��� �����:
							vector<string> valuesArrays = split(values,' ');
						
							outVertices.clear();
							for (int i = 0; i < numElements; i++) {
								/// 1 ������� ������������:
								Vertex p1;
								/// ������ ������� ���������� ��� 1 ������� ������������:
								GLuint posIndex1 = std::stoi(valuesArrays[0+(i*9)]);
								p1.position = coords[posIndex1];
								/// ������ ������� ���������� ������� ��� 1 ������� ������������:
								GLuint normalIndex1 = std::stoi(valuesArrays[1+(i*9)]);
								p1.normal = normals[normalIndex1];
								/// ������ ������� ���������� ���������� ��� 1 ������� ������������:
								GLuint uvIndex1 = std::stoi(valuesArrays[2+(i*9)]);
								p1.texCoords = uvCoords[uvIndex1];
								
								/// 2 ������� ������������:
								Vertex p2;
								/// ������ ������� ���������� ��� 2 ������� ������������:
								GLuint posIndex2 = std::stoi(valuesArrays[3+(i*9)]);
								p2.position = coords[posIndex2];
								/// ������ ������� ���������� ������� ��� 2 ������� ������������:
								GLuint normalIndex2 = std::stoi(valuesArrays[4+(i*9)]);
								p2.normal = normals[normalIndex2];
								/// ������ ������� ���������� ���������� ��� 2 ������� ������������:
								GLuint uvIndex2 = std::stoi(valuesArrays[5+(i*9)]);
								p2.texCoords = uvCoords[uvIndex2];
								
								/// 3 ������� ������������:
								Vertex p3;
								/// ������ ������� ���������� ��� 2 ������� ������������:
								GLuint posIndex3 = std::stoi(valuesArrays[6+(i*9)]);
								p3.position = coords[posIndex3];
								/// ������ ������� ���������� ������� ��� 2 ������� ������������:
								GLuint normalIndex3 = std::stoi(valuesArrays[7+(i*9)]);
								p3.normal = normals[normalIndex3];
								/// ������ ������� ���������� ���������� ��� 2 ������� ������������:
								GLuint uvIndex3 = std::stoi(valuesArrays[8+(i*9)]);
								p3.texCoords = uvCoords[uvIndex3];

								/// ��������� ����������� �� 3 ������:
								outVertices.push_back(p1);
								outVertices.push_back(p2);
								outVertices.push_back(p3);
							}
						}
					}

					/// ����������� - �������������� ������ OpenGL � �������� ����������� ������ � OpenGL:
					glGenVertexArrays(1, &VAO);
					glGenBuffers(1, &VBO);

					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					/// ������������� ���� ������� � OpenGL:
					glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(Vertex), &outVertices[0], GL_STATIC_DRAW);

					/// ���������� �������� ��������� ������:
					glEnableVertexAttribArray(0);	
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
					/// ���������� �������� �������:
					glEnableVertexAttribArray(1);	
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
					/// ���������� �������� ���������� ���������:
					glEnableVertexAttribArray(2);	
					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
					glBindVertexArray(0);					
				}
			}
		}
	}

	/// �������������� ������:
	shader.setup("data\\simpleShader2.vert","data\\simpleShader2.frag");
	loaderImage loader;
	diffuseTexture = loader.loadDDS(folder + "\\diffuse_map.dds");
	/// �������������� ��������� ������� ���������:
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

