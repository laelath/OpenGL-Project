#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

#include "shader.h"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line)) VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		cerr << "Cannot open: " << vertex_file_path << endl;
		return 0;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line)) FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling shader : " << vertex_file_path;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		cerr << &VertexShaderErrorMessage[0] << endl;
	}

	// Compile Fragment Shader
	cout << "Compiling shader : " << fragment_file_path;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		cerr << &FragmentShaderErrorMessage[0] << endl;
	}
	
	// Link the program
	cout << "Linking program" << endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		cerr << &ProgramErrorMessage[0] << endl;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void initShadowBuffers(point_light* l)
{
	glGenTextures(1, &l->shadowcubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, l->shadowcubemap);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, 1024, 1024, 0, GL_DEPTH_COMPONENT24, GL_FLOAT, 0);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	/*glGenFramebuffers(6, &l->framebuffers[0]);
	for (int i = 0; i < 6; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, l->framebuffers[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0);
		glDrawBuffer(GL_NONE);
	}*/
	glGenFramebuffers(1, &l->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, l->framebuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, l->shadowcubemap, 0);
	glDrawBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Shader::Shader(GLuint program)
{
	this->id = program;
}

Shader::Shader(const char* vertex_file_path, const char* fragment_file_path)
{
	id = LoadShaders(vertex_file_path, fragment_file_path);
}

void Shader::uniform1i(int i, string name)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform1iv(loc, 1, &i);
}

void Shader::uniform1f(float f, string name)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform1fv(loc, 1, &f);
}

void Shader::uniform2f(vec2 vec, string name)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform2fv(loc, 1, &vec[0]);
}

void Shader::uniform3f(vec3 vec, string name)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform3fv(loc, 1, &vec[0]);
}

void Shader::uniform4f(vec4 vec, string name)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniform4fv(loc, 1, &vec[0]);
}

void Shader::uniformMatrix4f(mat4 mat, string name)
{
	int loc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::uniformPtLight(point_light l, string name)
{
	int ploc = glGetUniformLocation(id, (name + ".position").c_str());
	glUniform3fv(ploc, 1, &l.position[0]);

	int cloc = glGetUniformLocation(id, (name + ".color").c_str());
	glUniform4fv(cloc, 1, &l.color[0]);
}