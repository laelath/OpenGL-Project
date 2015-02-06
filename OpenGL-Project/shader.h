#pragma once

#include <string>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

#include "light.h"

class Shader
{
public:
	Shader(GLuint program);
	Shader(string vertex_file_path, string fragment_file_path);

	GLuint getID();

	void uniform1i(int i, string name);
	void uniform1f(float f, string name);
	void uniform2f(vec2 vec, string name);
	void uniform3f(vec3 vec, string name);
	void uniform4f(vec4 vec, string name);
	void uniformMatrix4f(mat4 mat, string name);
	void uniformLight(Point_Light l, string name, mat4 viewMatrix);
	void uniformLight(Directional_Light l, string name, mat4 viewMatrix, int texture_handle);

private:
	GLuint id;
};

GLuint LoadShaders(string vertex_file_path, string fragment_file_path);