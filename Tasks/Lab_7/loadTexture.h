#pragma once

#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GlslShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

/// Вспомогательная библиотека для загрузки изображений:
class loaderImage{
	public:
		/// Загрузить пиксели из сжатого DXT-5 изображения в формате DDS:
		unsigned char* loadPixelsFromDDSFile(string fileName, unsigned int& width, unsigned int& height, unsigned int& format, unsigned int& size){
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
			height			= *(unsigned int*)&(header[8 ]);
			width			= *(unsigned int*)&(header[12]);
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

			/// Возвращаем полученный результат:
			unsigned int blockSize	= (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			size = ((width + 3 )/4)*((height+3)/4)*blockSize;

			return bufferData;
		}


		/// Загрузка сжатой текстуры с mip-map уровнями:
		GLuint loadDDS(string fileName){
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

};