#ifndef SHADER_H
#define SHADER_H

/*struct point_light
{
	vec3 position;
	vec4 color;


};*/

const unsigned int shadowResolution = 4096;

class Light
{
public:
	vec4 color;

protected:
	Light(vec4 color);
};

class Shadow_Light
{
public:
	GLuint getFramebufferID();
	GLuint getTextureID();

	mat4 getViewMatrix();

	virtual void updateMatrices() = 0;

protected:
	GLuint framebufferID, depthTextureID;

	mat4 viewMatrix;

	//Shadow_Light();

	virtual void initDepthBuffers() = 0;
};

class Point_Light : public Light, public Shadow_Light
{
public:
	vec3 position;

	Point_Light();
	Point_Light(vec3 position, vec4 color);

	mat4 getProjectionMatrix(int index);

	void updateMatrices();

protected:
	void initDepthBuffers();

private:
	mat4 projectionMatrices[6];
};

class Directional_Light : public Light, public Shadow_Light
{
public:
	vec3 direction;

	Directional_Light();
	Directional_Light(vec3 direction, vec4 color);

	mat4 getProjectionMatrix();
	mat4 getViewProjectionMatrix();

	void updateMatrices();

protected:
	void initDepthBuffers();

private:
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
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
