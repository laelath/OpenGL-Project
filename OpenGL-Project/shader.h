#ifndef SHADER_H
#define SHADER_H

struct point_light
{
	vec3 position;
	vec4 color;
	GLuint shadowcubemap;
	GLuint framebuffer/*s[6]*/;
};

class Shader
{
public:
	Shader(GLuint program);
	Shader(const char* vertex_file_path, const char* fragment_file_path);

	GLuint id;

	void uniform1i(int i, string name);
	void uniform1f(float f, string name);
	void uniform2f(vec2 vec, string name);
	void uniform3f(vec3 vec, string name);
	void uniform4f(vec4 vec, string name);
	void uniformMatrix4f(mat4 mat, string name);
	void uniformPtLight(point_light l, string name);
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

//void UpdateShadowmaps(point_light l, Shader program);

GLuint initShadowBuffers();

#endif
