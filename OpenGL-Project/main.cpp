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

vec3 ambient_model = vec3(0.005f, 0.025f, 0.1f);
//vec3 ambient_model = vec3(0.1f, 0.5f, 1.0f);

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
	glCullFace(GL_BACK);
	glClearColor(ambient_model.r, ambient_model.g, ambient_model.b, 0.1f);

	//Shader program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");
	//TESTING SHADERS
	Shader program("../resources/shaders/shader.vert", "../resources/shaders/simple.frag");
	Shader texturepass("../resources/shaders/texture.vert", "../resources/shaders/texture.frag");
	Shader depthProgram("../resources/shaders/depth.vert", "../resources/shaders/depth.frag");
	//

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

	model torus, trashbin, floor;
	load3DFromFile("../resources/models/torusball/torusball.obj", &torus);
	load3DFromFile("../resources/models/trashcan/trashbin.obj", &trashbin);
	load3DFromFile("../resources/models/floor/floor.obj", &floor);

	light lit;
	lit.position = vec3(-20, 100, 0);
	lit.color = vec4(1.0, 0.9, 0.7, 150);

	//TEMP CODE ************************************************************************************************************************************************************************************

	GLuint test_texture = loadTexture2D("../resources/textures/testtexture.tga");

	float quad_vertex_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	
	GLuint quad_vertex_array, quad_vertex_buffer;

	glGenVertexArrays(1, &quad_vertex_array);
	glBindVertexArray(quad_vertex_array);

	glGenBuffers(1, &quad_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_data), quad_vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int shadowResolution = 8192;

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cerr << "Error creating framebuffer." << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	vec3 lightDirection = vec3(1.0, 1.0, 1.0);
	vec4 lightColor = vec4(0.8, 0.7, 0.5, 1.0);

	//END OF TEMP CODE *****************************************************************************************************************************************************************************

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		computeMatrices(window);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glViewport(0, 0, shadowResolution, shadowResolution);

		glClear(GL_DEPTH_BUFFER_BIT);

		//glCullFace(GL_FRONT);
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			lightDirection = normalize(getPlayerPos());
		}

		mat4 depthProjection = ortho(-300.0f, 300.0f, -300.0f, 300.0f, -300.0f, 300.0f);
		mat4 depthView = lookAt(lightDirection, vec3(0, 0, 0), vec3(0, 1, 0));
		mat4 depthModel = mat4(1.0f);
		mat4 depthModelViewProjection = depthProjection * depthView * depthModel;

		glUseProgram(depthProgram.id);

		depthProgram.uniformMatrix4f(depthModelViewProjection, "modelViewProjection");
		
		glBindVertexArray(model_vao);

		drawModel(&torus, depthProgram);
		drawModel(&floor, depthProgram);
		drawModel(&trashbin, depthProgram);

		glBindVertexArray(0);



		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);

		//computeMatrices(window);
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		//mat4 projection = depthProjection;
		//mat4 view = depthView;
		mat4 model = mat4(1.0f);

		mat4 modelView = view * model;
		mat4 modelViewProjection = projection * view * model;
		mat4 depthBiasModelViewProjection = biasMatrix * depthModelViewProjection;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);

		glUseProgram(program.id);

		program.uniformMatrix4f(modelViewProjection, "modelViewProjection");
		program.uniformMatrix4f(modelView, "modelView");
		program.uniformLight(lit, "light[0]");
		program.uniform3f(ambient_model, "ambient_model");

		program.uniformMatrix4f(depthBiasModelViewProjection, "depthBiasMVP");

		//
		program.uniform3f(lightDirection, "lit.direction");
		program.uniform4f(lightColor, "lit.color");
		//

		glActiveTexture(GL_TEXTURE1);
		glBindSampler(1, shadow_sampler);

		glBindTexture(GL_TEXTURE_2D, depthTexture);
		program.uniform1i(1, "depth_texture");

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);

		glBindVertexArray(model_vao);

		drawModel(&torus, program);
		drawModel(&floor, program);
		drawModel(&trashbin, program);

		glBindVertexArray(0);

		//
		glViewport(0, 0, 384, 384);

		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(texturepass.id);

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, shadow_sampler);
		
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		texturepass.uniform1i(0, "Texture");

		glBindVertexArray(quad_vertex_array);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		//*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}