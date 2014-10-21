#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

GLuint loadGLpng(const char* file_name);

GLuint decodeGLpng(std::vector<unsigned char> png_data);

#endif