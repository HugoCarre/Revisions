#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <corecrt_memcpy_s.h>

class BMP
{
public:
static unsigned char *loadBMP_raw(const char *imagepath, unsigned int &width, unsigned int &height, bool flipY=true) {
	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file;
	fopen_s(&file, imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	// https://github.com/mortennobel/OpenGL_3_2_Utils/blob/master/src/TextureLoader.cpp
	if (flipY) {
		// swap y-axis
		unsigned char * tmpBuffer = new unsigned char[width * 3];
		int size = width * 3;
		for (unsigned int i = 0; i<height / 2; i++) {
			// copy row i to tmp
			memcpy_s(tmpBuffer, size, data + width * 3 * i, size);
			// copy row h-i-1 to i
			memcpy_s(data + width * 3 * i, size, data + width * 3 * (height - i - 1), size);
			// copy tmp to row h-i-1
			memcpy_s(data + width * 3 * (height - i - 1), size, tmpBuffer, size);
		}
		delete[] tmpBuffer;
	}

	return data;
}

// https://github.com/gbersac/mesh_renderer-42/blob/master/examples/opengl_tuto_org/common/texture.cpp
static GLuint loadBMP_texture(const char * imagepath, unsigned int &width, unsigned int &height, bool flipY = true) {
	unsigned char *data = loadBMP_raw(imagepath, width, height, flipY);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Return the ID of the texture we just created
	return textureID;
}

/*
// https://github.com/mortennobel/OpenGL_3_2_Utils/blob/master/src/TextureLoader.cpp
unsigned char * loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight, bool flipY) {
printf("Reading image %s\n", imagepath);
outWidth = -1;
outHeight = -1;
// Data read from the header of the BMP file
unsigned char header[54];
unsigned int dataPos;
unsigned int imageSize;
// Actual RGB data
unsigned char * data;

// Open the file
FILE * file;
fopen_s(&file, imagepath, "rb");
if (!file) { printf("Image could not be opened\n"); return NULL; }

// Read the header, i.e. the 54 first bytes

// If less than 54 byes are read, problem
if (fread(header, 1, 54, file) != 54) {
printf("Not a correct BMP file\n");
return NULL;
}
// A BMP files always begins with "BM"
if (header[0] != 'B' || header[1] != 'M') {
printf("Not a correct BMP file\n");
return NULL;
}
// Make sure this is a 24bpp file
if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return NULL; }
if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return NULL; }

// Read the information about the image
dataPos = *(int*)&(header[0x0A]);
imageSize = *(int*)&(header[0x22]);
outWidth = *(int*)&(header[0x12]);
outHeight = *(int*)&(header[0x16]);

// Some BMP files are misformatted, guess missing information
if (imageSize == 0)    imageSize = outWidth * outHeight * 3; // 3 : one byte for each Red, Green and Blue component
if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

// Create a buffer
data = new unsigned char[imageSize];

// Read the actual data from the file into the buffer
fread(data, 1, imageSize, file);

// Everything is in memory now, the file wan be closed
fclose(file);

if (flipY) {
// swap y-axis
unsigned char * tmpBuffer = new unsigned char[outWidth * 3];
int size = outWidth * 3;
for (unsigned int i = 0; i<outHeight / 2; i++) {
// copy row i to tmp
memcpy_s(tmpBuffer, size, data + outWidth * 3 * i, size);
// copy row h-i-1 to i
memcpy_s(data + outWidth * 3 * i, size, data + outWidth * 3 * (outHeight - i - 1), size);
// copy tmp to row h-i-1
memcpy_s(data + outWidth * 3 * (outHeight - i - 1), size, tmpBuffer, size);
}
delete[] tmpBuffer;
}

return data;
}
*/
};
