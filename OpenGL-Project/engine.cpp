#include <iostream>
#include <map>
#include <string>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "engine.h"

int state;
GLuint sampler;

map<string, Scene*> scenes;
map<string, Shader*> shaders;
map<string, Camera*> cameras;
map<string, Window*> windows;

void initEngine()
{
	initLightRenderData();

	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	if (!loadDefaultTexture("../resources/textures/default.png")) submit("imdt abort failed to load default texture.");
}

void initGL(int width, int height, string name, bool fullscreen)
{
	if (!glfwInit())
	{
		cerr << "GLFW failed to initialize." << endl;
		abort();
	}

	Window* window = new Window(width, height, name, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	window->makeContextCurrent();
	windows["default"] = window;

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		cerr << "Failed to initialize GLEW." << endl;
		abort();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void terminateEngine()
{
	for (pair<string, Scene*> scene : scenes) delete(scene.second);
	for (pair<string, Shader*> shader : shaders) delete(shader.second);
	for (pair<string, Camera*> camera : cameras) delete(camera.second);
	for (pair<string, Window*> window : windows) delete(window.second);
}