#include <iostream>
#include <fstream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

GLuint loadShader(string file_path, GLuint shader)
{
	GLuint shaderID = glCreateShader(shader);

	string shaderCode;
	ifstream shaderStream(file_path, std::ios::in);
	if (shaderStream.is_open())
	{
		string Line = "";
		while (getline(shaderStream, Line)) shaderCode += "\n" + Line;
		shaderStream.close();
	}
	else
	{
		cerr << "Cannot open: " << file_path << endl;
		return 0;
	}

	cout << "Compiling shader : " << file_path << endl;
	char const* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	GLint result = 0;

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		vector<char> shaderErrorMessage(infoLogLength);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
		cerr << &shaderErrorMessage[0] << endl;
		return 0;
	}

	return shaderID;
}

Shader::Shader(GLuint program)
{
	id = program;
}

Shader::Shader(string vertex_file_path, string fragment_file_path)
{
	GLuint vertexShaderID = loadShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = loadShader(fragment_file_path, GL_FRAGMENT_SHADER);

	cout << "Linking program" << endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		cerr << "Error Linking Program." << endl;
	}

	cout << endl;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	id = programID;
}

Shader::Shader(string vertex_file_path, string geometry_file_path, string fragment_file_path)
{
	GLuint vertexShaderID = loadShader(vertex_file_path, GL_VERTEX_SHADER);
	GLuint geometryShaderID = loadShader(geometry_file_path, GL_GEOMETRY_SHADER);
	GLuint fragmentShaderID = loadShader(fragment_file_path, GL_FRAGMENT_SHADER);

	cout << "Linking program" << endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, geometryShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		cerr << "Error Linking Program." << endl;
	}

	cout << endl;

	glDeleteShader(vertexShaderID);
	glDeleteShader(geometryShaderID);
	glDeleteShader(fragmentShaderID);

	id = programID;
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

GLuint Shader::getID() const
{
	return id;
}

void Shader::uniform1i(int i, string name) const
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform1iv(loc, 1, &i);
}

void Shader::uniform1f(float f, string name) const
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform1fv(loc, 1, &f);
}

void Shader::uniform2f(vec2 vec, string name) const
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform2fv(loc, 1, &vec[0]);
}

void Shader::uniform3f(vec3 vec, string name) const
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform3fv(loc, 1, &vec[0]);
}

void Shader::uniform4f(vec4 vec, string name) const
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform4fv(loc, 1, &vec[0]);
}

void Shader::uniformMatrix4f(mat4 mat, string name) const
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

/*void Shader::uniformLight(const Point_Light* l, string name, mat4 viewMatrix) const
{
	this->uniform3f((viewMatrix * vec4(l->position, 1)).xyz, name + ".position");
	this->uniform4f(l->color, name + ".color");
}

void Shader::uniformLight(const Directional_Light* l, string name, mat4 viewMatrix, GLuint sampler, unsigned int texture_handle) const
{
	this->uniform3f((viewMatrix * vec4(l->direction, 0)).xyz, name + ".direction");
	this->uniform4f(l->color, name + ".color");

	uniformMatrix4f(BIAS_MATRIX * l->getViewProjectionMatrix(), name + ".depthBiasMVP");

	glActiveTexture(GL_TEXTURE0 + texture_handle);
	glBindSampler(texture_handle, sampler);
	glBindTexture(GL_TEXTURE_2D, l->getTextureID());
	this->uniform1i(texture_handle, name + ".depth_texture");
}*/