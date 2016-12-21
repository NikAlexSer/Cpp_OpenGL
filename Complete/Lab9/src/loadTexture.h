#pragma once
#pragma warning(disable : 4996)

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


class loaderImage {
	public:
		unsigned char* loadPixelsFromDDSFile(string fileName, unsigned int& width,
			unsigned int& height, unsigned int& format, unsigned int& size) {
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			string exePathWithName = string(buffer);
			string::size_type pos = exePathWithName.find_last_of("\\/");
			string exePath = string(exePathWithName).substr(0, pos);
			string fullPath = exePath + "\\data\\" + fileName;

			/// Открываем файл
			FILE * file = fopen(fullPath.c_str(), "rb");
			if (!file) cout << "error load image! llolol" << endl;


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
			height = *(unsigned int*)&(header[8]);
			width = *(unsigned int*)&(header[12]);
			unsigned int linearSize = *(unsigned int*)&(header[16]);
			unsigned int mipMapCount = *(unsigned int*)&(header[24]);
			unsigned int fourCC = *(unsigned int*)&(header[80]);

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
			unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;

			/// Выбираем доступный формат OpenGL:	
			switch (fourCC) {
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
			unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;

			return bufferData;
		}

		GLuint loadDDS(string fileName) {
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			string exePathWithName = string(buffer);
			string::size_type pos = exePathWithName.find_last_of("\\/");
			string exePath = string(exePathWithName).substr(0, pos);
			string fullPath = exePath + "\\data\\" + fileName;
			FILE * file = fopen(fullPath.c_str(), "rb");
			if (!file) {
				cout << "error load image lol" << endl;
				return 0;
			}

			char filecode[4];
			fread(filecode, 1, 4, file);
			if (strncmp(filecode, "DDS ", 4) != 0) {
				fclose(file);
				cout << "extension file worng" << endl;
				cout << "error load image!" << endl;
				return 0;
			}

			unsigned char header[124];
			fread(&header, 124, 1, file);
			unsigned int height = *(unsigned int*)&(header[8]);
			unsigned int width = *(unsigned int*)&(header[12]);
			unsigned int linearSize = *(unsigned int*)&(header[16]);
			unsigned int mipMapCount = *(unsigned int*)&(header[24]);
			unsigned int fourCC = *(unsigned int*)&(header[80]);

			unsigned char * bufferData;
			unsigned int bufsize;
			bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
			bufferData = (unsigned char*)malloc(bufsize * sizeof(unsigned char));

			fread(bufferData, 1, bufsize, file);

			fclose(file);

			unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
			unsigned int format;

			switch (fourCC) {
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

			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;
			for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
				unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
				glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, bufferData + offset);
				offset += size;
				width /= 2;
				height /= 2;

			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			delete bufferData;
			return textureID;
		}

		GLuint loadBmp(string fileName) {
			//Получаем путь до проги, конвертируем, ищем слэш,
			//копируем в переменную exePath и формируем полный путь
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			string exePathWithName = string(buffer);
			string::size_type pos = exePathWithName.find_last_of("\\/");
			string exePath = string(exePathWithName).substr(0, pos);
			string fullPath = exePath + "\\data\\" + fileName;

			//открытие
			FILE * file = fopen(fullPath.c_str(), "rb");
			if (!file) {
				cout << "error load image!" << endl;
				return 0;
			}

			unsigned char header[54];
			if (fread(header, 1, 54, file) != 54) {
				cout << "error read header!" << endl;
			}
			if (header[0] != 'B' || header[1] != 'M') {
				cout << "not a correct bmp file" << endl;
				return 0;
			}
			unsigned int dataPos = *(int*)&(header[0x0A]);
			unsigned int imageSize = *(int*)&(header[0x22]);
			unsigned int width = *(int*)&(header[0x12]);
			unsigned int height = *(int*)&(header[0x16]);

			if (imageSize == 0) imageSize = width * height * 3;
			if (dataPos == 0) dataPos = 54;

			unsigned char * data = new unsigned char[imageSize];
			fread(data, 1, imageSize, file);
			fclose(file);

			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
			//Базовая фильтрация:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			delete data;
			return textureID;
		}
};