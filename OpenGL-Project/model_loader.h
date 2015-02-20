#pragma once

#include <vector>
#include <string>
using namespace std;

#include <GL/glew.h>

#include <glm/glm.hpp>
using namespace glm;

#include "shader.h"

struct vertex
{
	vec3 position;
	vec2 uv;
	vec3 normal;
};

struct material
{
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float opacity;
	float shininess;
	float shine_strength;
	GLuint texture;
};

struct mesh
{
	GLuint ibo;
	vector<unsigned int> indices;
	unsigned int material;
};

struct model
{
	GLuint vbo;
	vector<vertex> vertices;
	vector<material> materials;
	vector<mesh> meshes;
};

bool load3DFromFile(string path, model* lmodel);

void drawModel(const model* rmodel, const Shader* program);
void drawModel(const model* rmodel, const Shader* program, GLuint sampler, GLuint texture_handle);