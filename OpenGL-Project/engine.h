#pragma once

#include "console.h"
#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "window.h"
#include "input.h"
#include "image_loader.h"
#include "model_loader.h"

struct render_object
{
	Scene* scene;
	Shader* shader;
	Camera* camera;
	bool textured;
	bool lighting;
	bool clear_depth_buffer;
};

void initGL(int width, int height, string name, bool fullscreen);
void initEngine();

void addShader(string name, Shader* shader);