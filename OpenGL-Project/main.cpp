#include <iostream>
#include <vector>
#include <string>

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

vec3 ambient_model = vec3(0.01f, 0.05f, 0.1f);

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
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
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(ambient_model.r, ambient_model.g, ambient_model.b, 0.1f);

	Shader program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");
	//GLuint programID = LoadShaders("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	/*GLuint shadow_sampler;
	glGenSamplers(1, &shadow_sampler);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

	model torus, trashbin;
	load3DFromFile("../resources/torusball/torusball.obj", &torus);
	load3DFromFile("../resources/trashcan/trashbin.obj", &trashbin);

	//GLuint shadowmap = loadGLpng("../resources/shadowmap.png");

	light lit;
	lit.position = vec3(-20, 75, 0);
	lit.color = vec4(1.0, 0.9, 0.7, 10000);

	light endlit;
	endlit.position = vec3();
	endlit.color = vec4(0,0,0,-1);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		computeMatrices(window);
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);

		mat4 modelView = view * model;
		mat4 modelViewProjection = projection * view * model;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.id);

		program.uniformMatrix4f(modelViewProjection, "modelViewProjection");
		//GLuint mvpID = glGetUniformLocation(program.id, "modelViewProjection");
		//glUniformMatrix4fv(mvpID, 1, GL_FALSE, &modelViewProjection[0][0]);

		program.uniformMatrix4f(modelView, "modelView");
		//GLuint mvID = glGetUniformLocation(program.id, "modelView");
		//glUniformMatrix4fv(mvID, 1, GL_FALSE, &modelView[0][0]);

		program.uniformLight(lit, "light[0]");
		program.uniformLight(endlit, "light[1]");

		program.uniform3f(ambient_model, "ambient_model");
		//int mloc = glGetUniformLocation(programID, "ambient_model");
		//glUniform3f(mloc, ambient_model.r, ambient_model.g, ambient_model.b);

		/*glActiveTexture(GL_TEXTURE1);
		glBindSampler(1, shadow_sampler);

		glBindTexture(GL_TEXTURE_2D, shadowmap);*/

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);

		drawModel(&torus, program);
		drawModel(&trashbin, program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}