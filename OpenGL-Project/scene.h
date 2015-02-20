#pragma once

#include <string>
#include <vector>
using namespace std;

#include <GL/glew.h>

#include "camera.h"
#include "light.h"
#include "shader.h"
#include "model_loader.h"

class Scene
{
public:
	Scene();
	~Scene();

	void addModel(string path);
	void addModel(model* model);

	void addLight(Shadow_Light* light);

	void renderLights(const Shader* depth_program);

	void renderScene(const Shader* program, const Camera* camera) const;
	void renderScene(const Shader* program, const Camera* camera, GLuint shadow_sampler) const;
	void renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle) const;
	void renderScene(const Shader* program, const Camera* camera, GLuint sampler, GLuint texture_handle, GLuint shadow_sampler) const;

private:
	GLuint vao;

	vector<model*> models;
	vector<Shadow_Light*> lights;
};