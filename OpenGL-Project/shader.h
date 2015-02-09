#pragma once

#include <string>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

//#include "light.h"

class Shader
{
public:
	Shader(GLuint program);
	Shader(string vertex_file_path, string fragment_file_path);
	Shader(string vertex_file_path, string geometry_file_path, string fragment_file_path);

	~Shader();

	GLuint getID() const;

	void uniform1i(int i, string name) const;
	void uniform1f(float f, string name) const;
	void uniform2f(vec2 vec, string name) const;
	void uniform3f(vec3 vec, string name) const;
	void uniform4f(vec4 vec, string name) const;
	void uniformMatrix4f(mat4 mat, string name) const;
	//void uniformLight(const Point_Light* l, string name, mat4 viewMatrix) const;
	//void uniformLight(const Directional_Light* l, string name, mat4 viewMatrix, GLuint sampler, unsigned int texture_handle) const;

private:
	GLuint id;
};

//GLuint loadShaders(string vertex_file_path, string fragment_file_path);