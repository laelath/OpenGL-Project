#include <vector>
using namespace std;

#include "scene.h"

#define AMBIENT_MODEL vec3(0.08f, 0.11f, 0.14f)

Scene::Scene()
{
	glGenVertexArrays(1, &vao);
}

Scene::~Scene()
{
	for (Model* m : models)
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
	Model* nm = new Model(path);
	models.push_back(nm);
}

void Scene::addModel(Model* m)
{
	models.push_back(m);
}

void Scene::addLight(Shadow_Light* l)
{
	lights.push_back(l);
}

void Scene::renderLights()
{
	glUseProgram(DepthProgram()->getID());
	glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

	for (Shadow_Light* l : lights)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, l->getFramebufferID());
		glClear(GL_DEPTH_BUFFER_BIT);

		l->bindRenderData();

		glBindVertexArray(vao);

		for (Model* m : models)
		{
			m->draw(DepthProgram());
		}

		glBindVertexArray(0);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

/*void Scene::renderScene(const Shader* program, const Camera* camera) const
{
	glUseProgram(program->getID());

	program->uniform3f(AMBIENT_MODEL, "ambient_model");
	program->uniform1i(SHADOW_RESOLUTION, "depth_resolution");

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (Model* m : models)
	{
		m->draw(program);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}*/

void Scene::renderScene(const Shader* program, const Camera* camera, bool lighting) const
{
	glUseProgram(program->getID());

	program->uniform3f(AMBIENT_MODEL, "ambient_model");
	program->uniform1i(SHADOW_RESOLUTION, "depth_resolution");

	if (lighting)
	{
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			lights[i]->bindLight(program, i, camera->getViewMatrix(), i);
		}
	}

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (Model* m : models)
	{
		m->draw(program);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}

/*void Scene::renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle) const
{
	glUseProgram(program->getID());

	program->uniform3f(AMBIENT_MODEL, "ambient_model");
	program->uniform1i(SHADOW_RESOLUTION, "depth_resolution");

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (Model* m : models)
	{
		m->draw(program, sampler, texture_handle);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}*/

void Scene::renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle, bool lighting) const
{
	glUseProgram(program->getID());

	program->uniform3f(AMBIENT_MODEL, "ambient_model");
	program->uniform1i(SHADOW_RESOLUTION, "depth_resolution");

	if (lighting)
	{
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			lights[i]->bindLight(program, i, camera->getViewMatrix(), texture_handle + i + 1);
		}
	}

	program->uniformMatrix4f(camera->getViewProjectionMatrix(), "modelViewProjection");
	program->uniformMatrix4f(camera->getViewMatrix(), "modelView");

	glBindVertexArray(vao);

	for (Model* m : models)
	{
		m->draw(program, sampler, texture_handle);
	}

	glBindVertexArray(0);

	glUseProgram(0);
}