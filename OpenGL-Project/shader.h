#ifndef SHADER_H
#define SHADER_H

/*struct point_light
{
	vec3 position;
	vec4 color;


};*/

class Point_Light
{
public:
	Point_Light();
	Point_Light(vec3 position, vec4 color);

	vec3 position;
	vec4 color;

	GLuint getFramebufferID();
	GLuint getTextureID();

private:
	GLuint framebuffer;
	GLuint depthCubeTexture;

	void initDepthBuffers();
};

class Directional_Light
{
public:

};



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

private:
	GLuint id;
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif
