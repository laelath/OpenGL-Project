#pragma once

class Shader
{
public:
	Shader(GLuint program);
	Shader(const char* vertex_file_path, const char* fragment_file_path);

	GLuint getID();

	void uniform1i(int i, std::string name);
	void uniform1f(float f, std::string name);
	void uniform2f(vec2 vec, std::string name);
	void uniform3f(vec3 vec, std::string name);
	void uniform4f(vec4 vec, std::string name);
	void uniformMatrix4f(mat4 mat, std::string name);
	void uniformLight(Point_Light l, std::string name);
	void uniformLight(Directional_Light l, std::string name);

private:
	GLuint id;
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);