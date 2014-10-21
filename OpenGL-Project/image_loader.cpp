#include <iostream>
//#include <fstream>
#include <vector>
#include <string>

#include <GL/glew.h>

#include "lodepng.h"
#include "image_loader.h"

//#define PNGSIGSIZE 8

/*bool validate(std::istream& source)
{
	png_byte pngsig[PNGSIGSIZE];
	int is_png = 0;

	source.read((char*)pngsig, PNGSIGSIZE);

	if (!source.good()) return false;

	is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
	return (is_png == 0);
}*/

/*void userReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(png_ptr);

	((std::istream*)a)->read((char*)data, length);
}*/

GLuint decodeGLpng(std::vector<unsigned char> png_data)
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
	//std::ifstream in_file (file_name, std::ifstream::in);

	/*if (!validate(in_file))
	{
		std::cerr << "ERROR: Data is not valid PNG data" << std::endl;
		return 0;
	}*/

	/*FILE* in_file;
	fopen_s(&in_file, filename, "r");

	if (!in_file)
	{
		std::cerr << "ERROR: Couldn't open PNG file" << std::endl;
		return 0;
	}

	png_byte png_sig[PNGSIGSIZE];

	if (!png_sig_cmp(png_sig, 0, PNGSIGSIZE))
	{
		std::cerr << "ERROR: File not in PNG format" << std::endl;
		return 0;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		std::cerr << "ERROR: Couldn't initialize PNG read struct" << std::endl;
		return 0;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		std::cerr << "ERROR: Couldn't initialize PNG info struct" << std::endl;
		png_destroy_read_struct(&png_ptr, (png_infopp)0, (png_infopp)0);
		return 0;
	}

	png_bytep* row_ptrs = NULL;
	char* data = NULL;

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
		if (row_ptrs != NULL) delete[] row_ptrs;
		if (data != NULL) delete[] data;

		std::cout << "ERROR: An error occured while reading the PNG file";

		return 0;
	}

	//png_set_read_fn(png_ptr, (png_voidp)&in_file, userReadData);
	png_init_io(png_ptr, in_file);
	png_set_sig_bytes(png_ptr, PNGSIGSIZE);
	
	png_read_info(png_ptr, info_ptr);

	png_uint_32 imgWidth = png_get_image_width(png_ptr, info_ptr);
	png_uint_32 imgHeight = png_get_image_height(png_ptr, info_ptr);

	png_uint_32 bitdepth = png_get_bit_depth(png_ptr, info_ptr);
	
	png_uint_32 channels = png_get_channels(png_ptr, info_ptr);
	
	png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

	switch (color_type)
	{
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(png_ptr);
		channels = 3;
		break;
	case PNG_COLOR_TYPE_GRAY:
		if (bitdepth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
		bitdepth = 8;
		break;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
		channels += 1;
	}

	if (bitdepth == 16) png_set_strip_16(png_ptr);
	
	row_ptrs = new png_bytep[imgHeight];
	data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
	const unsigned int stride = imgWidth * bitdepth * channels / 8;

	for (size_t i; i < imgHeight; i++)
	{
		png_uint_32 q = (imgHeight - i - 1) * stride;
		row_ptrs[i] = (png_bytep)data + q;
	}

	png_read_image(png_ptr, row_ptrs);

	delete[](png_bytep)row_ptrs;

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);*/

	std::vector<unsigned char> data;
	//unsigned width, height;

	//unsigned error = lodepng::decode(data, width, height, filename);
	lodepng::load_file(data, filename);

	
	//if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	GLuint textureID = decodeGLpng(data);
	//glGenTextures(1, &textureID);
	//std::cout << "Texture ID " << std::to_string(textureID) << std::endl;

	//glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	
	/*switch (channels)
	{
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, imgWidth, imgHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	}*/

	return textureID;
}