#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include "input.h"
//#include "light.h"
//#include "shader.h"
#include "player.h"
//#include "model_loader.h"
//#include "image_loader.h"
//#include "scene.h"
//#include "camera.h"
//#include "console.h"
//#include "window.h"
#include "engine.h"

int main()
{
	initConsole();

	initGL(1280, 720, "C++ OpenGL", false);

	/*
	if (!glfwInit()) submit("imdt abort Failed to initialize GLFW.");

	Window window(1280, 720, "C++ OpenGL", NULL, NULL);
	window.makeContextCurrent();
	bindWindow(window.glfwWindow());

	glewExperimental = true;
	if (glewInit() != GLEW_OK) submit("imdt abort Failed to initialize GLEW.");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	*/

	initLightRenderData();

	Shader program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	
	if (!loadDefaultTexture("../resources/textures/default.png")) submit("imdt abort failed to load default texture.");

	Scene scene;
	scene.addModel("../resources/models/torusball/torusball.obj");
	scene.addModel("../resources/models/trashcan/trashbin.obj");
	scene.addModel("../resources/models/floor/floor.obj");

	//scene.addLight(new Point_Light(vec3(0.0, 75.0, 0.0), vec4(0.8, 0.8, 0.8, 100)));
	scene.addLight(new Directional_Light(vec3(1.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 0.5)));
	scene.addLight(new Directional_Light(vec3(1.0, 0.7, -0.5), vec4(0.0, 1.0, 0.0, 0.5)));
	scene.addLight(new Directional_Light(vec3(-1.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.5)));

	//Scene gui;


	Perspective_Camera camera(90.0f, 1280.0f / 720.0f, 0.1f, 100000.0f);
	Player player(&camera);

	int game_state = 0;
	setMouseLock(true);

	double lastTime = glfwGetTime();

	while (game_state != -1 && !isExitRequested())
	{
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		execute_queue();

		if (game_state == 0)
		{
			player.update(delta);
		}

		scene.renderLights();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int width, height;
		glfwGetFramebufferSize(window.glfwWindow(), &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene.renderScene(&program, &camera, sampler, 0, true);

		window.update();

		updateInput();

		if (isKeyPressed(GLFW_KEY_ESCAPE))
		{
			if (game_state == 0)
			{
				game_state = 1;
				setMouseLock(false);
			} 
			else if (game_state == 1)
			{
				game_state = 0;
				setMouseLock(true);
			}
		}
	}
	glfwTerminate();
}