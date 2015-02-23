#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "input.h"
#include "light.h"
#include "shader.h"
#include "player.h"
#include "model_loader.h"
#include "image_loader.h"
#include "scene.h"
#include "camera.h"

vec3 ambient_model = vec3(0.08f, 0.11f, 0.14f);

int main()
{
	if (!glfwInit())
	{
		cerr << "Failed to initialize GLFW." << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(1280, 720, "C++ OpenGL", NULL, NULL);

	if (window == NULL){
		cerr << "Failed to open GLFW window." << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	initInput(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW." << endl;
		return -1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(ambient_model.r, ambient_model.g, ambient_model.b, 0.1f);

	Shader program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");
	Shader depthProgram("../resources/shaders/depth.vert", "../resources/shaders/depth.frag");
	Shader passProgram("../resources/shaders/texture.vert", "../resources/shaders/texture.frag");

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	GLuint shadow_sampler;
	glGenSamplers(1, &shadow_sampler);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	Scene scene;

	scene.addModel("../resources/models/torusball/torusball.obj");
	scene.addModel("../resources/models/trashcan/trashbin.obj");
	scene.addModel("../resources/models/floor/floor.obj");

	scene.addLight(new Directional_Light(vec3(1.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 0.5)));
	scene.addLight(new Directional_Light(vec3(1.0, 0.7, -0.5), vec4(0.0, 1.0, 0.0, 0.5)));
	scene.addLight(new Directional_Light(vec3(-1.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.5)));

	Perspective_Camera camera(vec3(), vec3(), 90.0f, 1280.0f / 720.0f, 0.1f, 100000.0f);
	Player player(&camera);

	double lastTime = glfwGetTime();

	while (!isKeyPressed(GLFW_KEY_ESCAPE) && glfwWindowShouldClose(window) == 0)
	{
		//computeMatrices(window);
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		player.update(delta);

		//camera.setPosition(getPlayerPos());
		//camera.setRotation(getPlayerViewAngles());

		scene.renderLights(&depthProgram);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.uniform3f(ambient_model, "ambient_model");
		program.uniform1i(SHADOW_RESOLUTION, "depth_resolution");

		scene.renderScene(&program, &camera, sampler, 0, shadow_sampler);

		glfwSwapBuffers(window);
		updateInput();
	}

	glfwTerminate();

	return 0;
}