#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

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
	std::vector<unsigned int> indices;
	unsigned int material;
};

struct model
{
	GLuint vao;
	GLuint vbo;
	std::vector<vertex> vertices;
	std::vector<material> materials;
	std::vector<mesh> meshes;
};

bool load3DFromFile(const char* pFile, model* lmodel);

void drawModel(const model* rmodel, Shader program);

#endif