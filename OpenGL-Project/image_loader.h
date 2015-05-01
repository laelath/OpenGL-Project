#pragma once

#include <string>
using namespace std;

#include <GL/glew.h>

void setDefaultTexture(GLuint texID);
GLuint loadTexture(string filename);

//GLuint loadGLpng(const char* file_name);

//GLuint decodeGLpng(std::vector<unsigned char> png_data);