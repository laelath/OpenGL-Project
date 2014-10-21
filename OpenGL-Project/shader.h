#ifndef SHADER_H
#define SHADER_H

struct light
{
	vec3 position;
	vec4 color;
};

class Shader
{
public:
	Shader(GLuint program);
	Shader(const char* vertex_file_path, const char* fragment_file_path);

	GLuint id;

	void uniform1f(float f, std::string name);
	void uniform2f(vec2 vec, std::string name);
	void uniform3f(vec3 vec, std::string name);
	void uniform4f(vec4 vec, std::string name);
	void uniformMatrix4f(mat4 mat, std::string name);
	void uniformLight(light l, std::string name);
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

//void BindLight(GLuint shader, string name, light l);
//void BindLight(Shader shader, string name, light l);

#endif
