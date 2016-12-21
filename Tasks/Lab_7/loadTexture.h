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

/// ��������������� ���������� ��� �������� �����������:
class loaderImage{
	public:
		/// ��������� ������� �� ������� DXT-5 ����������� � ������� DDS:
		unsigned char* loadPixelsFromDDSFile(string fileName, unsigned int& width, unsigned int& height, unsigned int& format, unsigned int& size){
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
			height			= *(unsigned int*)&(header[8 ]);
			width			= *(unsigned int*)&(header[12]);
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

			/// ���������� ���������� ���������:
			unsigned int blockSize	= (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			size = ((width + 3 )/4)*((height+3)/4)*blockSize;

			return bufferData;
		}


		/// �������� ������ �������� � mip-map ��������:
		GLuint loadDDS(string fileName){
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

};