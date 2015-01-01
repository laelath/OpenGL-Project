#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.h"
#include "controls.h"
#include "model_loader.h"
#include "image_loader.h"

//vec3 ambient_model = vec3(0.01f, 0.05f, 0.2f);
vec3 ambient_model = vec3(0.0f, 0.0f, 0.0f);

int main()
{
	if (!glfwInit())
	{
		cerr << "Failed to initialize GLFW." << endl;
		return (-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "C++ OpenGL", NULL, NULL);

	if (window == NULL){
		cerr << "Failed to open GLFW window." << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
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
	glClearColor(ambient_model.r, ambient_model.g, ambient_model.b, 0.1f);

	Shader program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");
	Shader shadow_program("../resources/shaders/shadows.vert", "../resources/shaders/shadows.frag", "../resources/shaders/shadows.geom");

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	model torus, trashbin, floor;
	load3DFromFile("../resources/torusball/torusball.obj", &torus);
	load3DFromFile("../resources/trashcan/trashbin.obj", &trashbin);
	load3DFromFile("../resources/floor/floor.obj", &floor);

	point_light lit;
	lit.position = vec3(-20, 100, 0);
	lit.color = vec4(1.0, 0.9, 0.7, 150);
	initShadowBuffers(&lit);
	updateMatrices(&lit);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, lit.framebuffer);
		glViewport(0, 0, 1024, 1024);

		glCullFace(GL_FRONT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shadow_program.id);

		for (int i = 0; i < 6; i++)
		{
			shadow_program.uniformMatrix4f(lit.projectionMatrices[i], "shadowmapProjections[" + to_string(i) + "]");
		}

		drawModel(&torus, shadow_program);
		drawModel(&trashbin, shadow_program);
		drawModel(&floor, shadow_program);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);

		computeMatrices(window);
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);

		mat4 modelView = view * model;
		mat4 modelViewProjection = projection * view * model;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.id);

		program.uniformMatrix4f(modelViewProjection, "modelViewProjection");
		program.uniformMatrix4f(modelView, "modelView");
		program.uniformLight(lit, "light[0]");
		program.uniform3f(ambient_model, "ambient_model");

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);

		drawModel(&torus, program);
		drawModel(&trashbin, program);
		drawModel(&floor, program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}