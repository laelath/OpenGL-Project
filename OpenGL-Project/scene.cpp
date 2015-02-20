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

void Scene::renderLights(const Shader* depth_program)
{
	for (Shadow_Light* l : lights)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, l->getFramebufferID());

		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(depth_program->getID());

		l->bindRenderData(depth_program);

		glBindVertexArray(vao);

		for (model* m : models)
		{
			drawModel(m, depth_program);
		}

		glBindVertexArray(0);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Scene::renderScene(const Shader* program, const Camera* camera) const
{
	glUseProgram(program->getID());

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (model* m : models)
	{
		drawModel(m, program);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}

void Scene::renderScene(const Shader* program, const Camera* camera, GLuint shadow_sampler) const
{
	glUseProgram(program->getID());
	
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		lights[i]->bindLight(program, i, camera->getViewMatrix(), shadow_sampler, i);
	}

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (model* m : models)
	{
		drawModel(m, program);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}

void Scene::renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle) const
{
	glUseProgram(program->getID());

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (model* m : models)
	{
		drawModel(m, program, sampler, texture_handle);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}

void Scene::renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle, GLuint shadow_sampler) const
{
	glUseProgram(program->getID());

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		lights[i]->bindLight(program, i, camera->getViewMatrix(), shadow_sampler, texture_handle + i + 1);
	}

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (model* m : models)
	{
		drawModel(m, program, sampler, texture_handle);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}