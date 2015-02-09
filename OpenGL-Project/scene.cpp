#include <vector>
using namespace std;

#include "scene.h"

Scene::Scene()
{
	glGenVertexArrays(1, &vao);
}

Scene::~Scene()
{
	for (model* m : models)
	{
		delete(m);
	}

	for (Shadow_Light* l : lights)
	{
		delete(l);
	}

	glDeleteVertexArrays(1, &vao);
}

void Scene::addModel(string path)
{
	model* nm = new model;
	if (load3DFromFile(path, nm))
	{
		models.push_back(nm);
	}
}

void Scene::addModel(model* m)
{
	models.push_back(m);
}

void Scene::addLight(Shadow_Light* l)
{
	lights.push_back(l);
}

void Scene::renderScene(const Shader* program, const Camera* camera, bool lighting) const
{
	if (lighting)
	{
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			//lights[i]->bindLight(program, "", )
		}
	}

	glBindVertexArray(vao);

	for (unsigned int i = 0; i < models.size(); i++)
	{
		drawModel(models[i], program);
	}

	glBindVertexArray(0);
}

void Scene::renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle, bool lighting) const
{

}