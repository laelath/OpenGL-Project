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

#include "light.h"
#include "shader.h"
#include "controls.h"
#include "model_loader.h"
#include "image_loader.h"

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

	GLuint model_vao;
	glGenVertexArrays(1, &model_vao);

	//TEMP////////////////////////////////////////////////////
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	//TEMP/////////////////////////////////////////////////////

	model torus, trashbin, floor;
	load3DFromFile("../resources/models/torusball/torusball.obj", &torus);
	load3DFromFile("../resources/models/trashcan/trashbin.obj", &trashbin);
	load3DFromFile("../resources/models/floor/floor.obj", &floor);

	Point_Light lit(vec3(-20, 100, 0), vec4(1.0, 0.9, 0.7, 100));
	Directional_Light d0lit;
	Directional_Light d1lit(vec3(1.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 0.5));
	Directional_Light d2lit(vec3(1.0, 0.7, -0.5), vec4(0.0, 1.0, 0.0, 0.5));
	Directional_Light d3lit(vec3(-1.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.5));
	int light = 1;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		computeMatrices(window);

		glBindFramebuffer(GL_FRAMEBUFFER, d1lit.getFramebufferID());

		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

		glClear(GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			d1lit.direction = normalize(getPlayerPos());
			d1lit.updateMatrices();
		}

		glUseProgram(depthProgram.getID());

		depthProgram.uniformMatrix4f(d1lit.getViewProjectionMatrix(), "modelViewProjection");
		
		glBindVertexArray(model_vao);

		drawModel(&torus, depthProgram);
		drawModel(&floor, depthProgram);
		drawModel(&trashbin, depthProgram);

		glBindVertexArray(0);





		glBindFramebuffer(GL_FRAMEBUFFER, d2lit.getFramebufferID());

		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

		glClear(GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			d2lit.direction = normalize(getPlayerPos());
			d2lit.updateMatrices();
		}

		glUseProgram(depthProgram.getID());

		depthProgram.uniformMatrix4f(d2lit.getViewProjectionMatrix(), "modelViewProjection");

		glBindVertexArray(model_vao);

		drawModel(&torus, depthProgram);
		drawModel(&floor, depthProgram);
		drawModel(&trashbin, depthProgram);

		glBindVertexArray(0);





		glBindFramebuffer(GL_FRAMEBUFFER, d3lit.getFramebufferID());

		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

		glClear(GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			d3lit.direction = normalize(getPlayerPos());
			d3lit.updateMatrices();
		}

		glUseProgram(depthProgram.getID());

		depthProgram.uniformMatrix4f(d3lit.getViewProjectionMatrix(), "modelViewProjection");

		glBindVertexArray(model_vao);

		drawModel(&torus, depthProgram);
		drawModel(&floor, depthProgram);
		drawModel(&trashbin, depthProgram);

		glBindVertexArray(0);


		//TEMP///////////////////////////////////////////
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			if (light == 1)
			{
				d0lit = d1lit;
				light++;
			}
			else if (light == 2)
			{
				d0lit = d2lit;
				light++;
			}
			else if (light == 3)
			{
				d0lit = d3lit;
				light++;
			}
			else if (light >= 4)
			{
				light = 1;
			}
		}
		//TEMP////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);

		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);

		mat4 modelView = view * model;
		mat4 modelViewProjection = projection * view * model;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.getID());

		program.uniformMatrix4f(modelViewProjection, "modelViewProjection");
		program.uniformMatrix4f(modelView, "modelView");
		program.uniform3f(ambient_model, "ambient_model");

		program.uniform1i(SHADOW_RESOLUTION, "depth_resolution");

		glActiveTexture(GL_TEXTURE1);
		glBindSampler(1, shadow_sampler);
		program.uniformLight(d1lit, "directionalLights[0]", modelView, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindSampler(2, shadow_sampler);
		program.uniformLight(d2lit, "directionalLights[1]", modelView, 2);

		glActiveTexture(GL_TEXTURE3);
		glBindSampler(3, shadow_sampler);
		program.uniformLight(d3lit, "directionalLights[2]", modelView, 3);

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);

		glBindVertexArray(model_vao);

		drawModel(&torus, program);
		drawModel(&floor, program);
		drawModel(&trashbin, program);

		glBindVertexArray(0);


		//TESTING////////////////////////////////////////
		glClear(GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, 256, 256);

		glUseProgram(passProgram.getID());

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, shadow_sampler);
		glBindTexture(GL_TEXTURE_2D, d3lit.getTextureID());
		passProgram.uniform1i(0, "Texture");

		glBindVertexArray(model_vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		//TESTING//////////////////////////////////////////

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}