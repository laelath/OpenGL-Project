#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.h"

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
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
	else
	{
		cerr << "Cannot open: " << fragment_file_path << endl;
		return 0;
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

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path, const char* geometry_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string line = "";
		while (getline(VertexShaderStream, line)) VertexShaderCode += "\n" + line;
		VertexShaderStream.close();
	}
	else
	{
		cerr << "Cannot open: " << vertex_file_path << endl;
		return 0;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string line = "";
		while (getline(FragmentShaderStream, line)) FragmentShaderCode += "\n" + line;
		FragmentShaderStream.close();
	}
	else
	{
		cerr << "Cannot open: " << fragment_file_path << endl;
		return 0;
	}

	// Read the Geometry Shader code from the file
	string GeometryShaderCode;
	ifstream GeometryShaderStream(geometry_file_path, ios::in);
	if (GeometryShaderStream.is_open())
	{
		string line = "";
		while (getline(GeometryShaderStream, line)) GeometryShaderCode += "\n" + line;
		GeometryShaderStream.close();
	}
	else
	{
		cerr << "Cannot open: " << geometry_file_path << endl;
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling shader : " << vertex_file_path;
	char const* VertexSourcePointer = VertexShaderCode.c_str();
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
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
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

	// Compile Geometry Shader
	cout << "Compiling shader : " << geometry_file_path;
	char const* GeometrySourcePointer = GeometryShaderCode.c_str();
	glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
	glCompileShader(GeometryShaderID);

	// Check Geometry Shader
	glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
		cerr << &GeometryShaderErrorMessage[0] << endl;
	}

	// Link the program
	cout << "Linking program" << endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glAttachShader(ProgramID, GeometryShaderID);
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
	glDeleteShader(GeometryShaderID);

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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	
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

void updateMatrices(point_light* l)
{
	l->projectionMatrices[0] = lookAt(l->position, l->position + vec3(1, 0, 0), vec3(0, 1, 0));
	l->projectionMatrices[1] = lookAt(l->position, l->position - vec3(1, 0, 0), vec3(0, 1, 0));
	l->projectionMatrices[2] = lookAt(l->position, l->position + vec3(0, 1, 0), vec3(0, 0, 1));
	l->projectionMatrices[3] = lookAt(l->position, l->position - vec3(0, 1, 0), vec3(0, 0, -1));
	l->projectionMatrices[4] = lookAt(l->position, l->position + vec3(0, 0, 1), vec3(0, 1, 0));
	l->projectionMatrices[5] = lookAt(l->position, l->position - vec3(0, 0, 1), vec3(0, 1, 0));

	mat4 projectionMatrix = perspective(radians(90.0f), 1.0f, 0.01f, 10000.0f);

	for (int i = 0; i < 6; i++)
	{
		l->projectionMatrices[i] = projectionMatrix * l->projectionMatrices[i];
	}
}

Shader::Shader(GLuint program)
{
	this->id = program;
}

Shader::Shader(const char* vertex_file_path, const char* fragment_file_path)
{
	id = LoadShaders(vertex_file_path, fragment_file_path);
}

Shader::Shader(const char* vertex_file_path, const char* fragment_file_path, const char* geometry_file_path)
{
	id = LoadShaders(vertex_file_path, fragment_file_path, geometry_file_path);
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

void Shader::uniformLight(point_light l, string name)
{
	int ploc = glGetUniformLocation(id, (name + ".position").c_str());
	glUniform3fv(ploc, 1, &l.position[0]);

	int cloc = glGetUniformLocation(id, (name + ".color").c_str());
	glUniform4fv(cloc, 1, &l.color[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, l.shadowcubemap);
	
	int tloc = glGetUniformLocation(id, (name + ".shadow_map").c_str());
	glUniform1i(tloc, l.shadowcubemap);
}