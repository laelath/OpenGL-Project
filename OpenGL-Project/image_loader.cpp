#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <FreeImage.h>

#include "image_loader.h"

vector<string> loadedpaths;
vector<GLuint> loadedIDs;

GLuint decodeTexture2D()
{
	return 0;
}

GLuint loadTexture2D(const char* filename)
{
	for (unsigned int i = 0; i < loadedpaths.size(); i++)
	{
		if (string(filename) == loadedpaths[i])
		{
			cout << "Found texture: " << filename << endl;
			return loadedIDs[i];
		}
	}

	FREE_IMAGE_FORMAT format;
	FIBITMAP* image = 0;

	format = FreeImage_GetFileType(filename, 0);

	if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(filename);
	if (format == FIF_UNKNOWN)
	{
		cerr << "Failed to load image: " << filename << ". Invalid file type." << endl;
		return false;
	}

	if (FreeImage_FIFSupportsReading(format)) image = FreeImage_Load(format, filename);
	if (!image)
	{
		cerr << "Failed to load image: " << filename << ". Error loading image." << endl;
		return false;
	}

	BYTE* pixels = FreeImage_GetBits(image);

	unsigned int width = FreeImage_GetWidth(image);
	unsigned int height = FreeImage_GetHeight(image);
	unsigned int bpp = FreeImage_GetBPP(image);

	if (pixels == NULL || width == 0 || height == 0)
	{
		cerr << "Failed to load image: " << filename << ". Error reading image." << endl;
		return false;
	}

	cout << "Loading texture: " << filename << endl;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int imgFormat = bpp == 32 ? GL_BGRA : bpp == 24 ? GL_BGR : bpp == 8 ? GL_LUMINANCE : 0;

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, imgFormat, GL_UNSIGNED_BYTE, pixels);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, DataPointer);

	loadedpaths.push_back(string(filename));
	loadedIDs.push_back(textureID);

	return textureID;
}


/*GLuint decodeGLpng(std::vector<unsigned char> png_data)
{
	std::vector<unsigned char> img;
	unsigned width, height;

	unsigned error = lodepng::decode(img, width, height, png_data);

	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);

	return textureID;
}

GLuint loadGLpng(const char* filename)
{

	std::vector<unsigned char> data;
	lodepng::load_file(data, filename);

	GLuint textureID = decodeGLpng(data);

	return textureID;
}*/