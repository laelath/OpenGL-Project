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

vec3 ambient_model = vec3(0.01f, 0.04f, 0.08f);
//vec3 ambient_model = vec3(0.0f, 0.0f, 0.0f);

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
	window = glfwCreateWindow(1280, 720, "C++ OpenGL", NULL, NULL);

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

	Shader program("../resources/shaders/simple.vert", "../resources/shaders/simple.frag");
	Shader shadow_program("../resources/shaders/testing.vert", "../resources/shaders/testing.frag");

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	model torus, trashbin, floor;
	load3DFromFile("../resources/models/torusball/torusball.obj", &torus);
	load3DFromFile("../resources/models/trashcan/trashbin.obj", &trashbin);
	load3DFromFile("../resources/models/floor/floor.obj", &floor);

	point_light lit;
	lit.position = vec3(-20, 100, 0);
	lit.color = vec4(1.0, 0.9, 0.7, 150);
	initShadowBuffers(&lit);
	updateMatrices(&lit);

	//TEMPORARY TESTING CODE!!!!!!!!!!!!
	//REMOVE AT SOMEPOINT
	
	GLuint framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	GLuint shadowSampler;
	glGenSamplers(1, &shadowSampler);
	glSamplerParameteri(shadowSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(shadowSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(shadowSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(shadowSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cerr << "Framebuffer failed to initialize." << endl;
	
	mat4 depthMVP = ortho<float>(-200, 200, 200, -200, -100, 100) * lookAt(vec3(-1, 2, 0), vec3(0,0,0), vec3(0,1,0));

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	mat4 depthBiasMVP = biasMatrix * depthMVP;

	//END OF TESTING CODE

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		/*glBindFramebuffer(GL_FRAMEBUFFER, lit.framebuffer);
		glViewport(0, 0, 1024, 1024);

		glCullFace(GL_FRONT);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shadow_program.id);

		for (int i = 0; i < 6; i++)
		{
			shadow_program.uniformMatrix4f(lit.projectionMatrices[i], "shadowmapProjections[" + to_string(i) + "]");
		}
		*/

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, 1024, 1024);

		glCullFace(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shadow_program.id);

		shadow_program.uniformMatrix4f(depthMVP, "depthMVP");

		drawModel(&torus, shadow_program);
		drawModel(&trashbin, shadow_program);
		drawModel(&floor, shadow_program);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		glViewport(0, 0, width, height);

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

		//TEMP
		program.uniform3f(vec3(1.0, 0.9, 0.7), "light_color");
		program.uniform3f(vec3(-1, 2, 0), "light_direction");
		program.uniformMatrix4f(depthBiasMVP, "depthBiasMVP");

		glActiveTexture(GL_TEXTURE1);
		glBindSampler(1, shadowSampler);

		glBindTexture(GL_TEXTURE_2D, depthTexture);
		program.uniform1i(1, "shadowMap");
		//END TEMP

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