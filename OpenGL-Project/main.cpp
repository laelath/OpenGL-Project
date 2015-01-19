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

//vec3 ambient_model = vec3(0.005f, 0.025f, 0.1f);
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

	Point_Light lit(vec3(-20, 100, 0), vec4(1.0, 0.9, 0.7, 150));
	Directional_Light dlit(vec3(1.0, 1.0, 1.0), vec4(0.8, 0.7, 0.5, 1.0));
	//point_light lit;
	//lit.position = vec3(-20, 100, 0);
	//lit.color = vec4(1.0, 0.9, 0.7, 150);

	/*/TEMP CODE ************************************************************************************************************************************************************************************

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//unsigned int shadowResolution = 8192;

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cerr << "Error creating framebuffer." << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//*/

	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	//vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
	//vec4 lightColor = vec4(0.8, 0.7, 0.5, 1.0);

	//END OF TEMP CODE *****************************************************************************************************************************************************************************

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		computeMatrices(window);

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, dlit.getFramebufferID());

		glViewport(0, 0, shadowResolution, shadowResolution);

		glClear(GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			//lightDirection = normalize(getPlayerPos());
			dlit.direction = normalize(getPlayerPos());
			dlit.updateMatrices();
		}

		//mat4 depthProjection = ortho(-300.0f, 300.0f, -300.0f, 300.0f, -300.0f, 300.0f);
		//mat4 depthView = lookAt(lightDirection, vec3(0,0,0), vec3(0,1,0));
		//mat4 depthModelViewProjection = depthProjection * depthView;

		glUseProgram(depthProgram.getID());

		//depthProgram.uniformMatrix4f(depthModelViewProjection, "modelViewProjection");
		depthProgram.uniformMatrix4f(dlit.getProjectionMatrix() * dlit.getViewMatrix(), "modelViewProjection");
		
		glBindVertexArray(model_vao);

		drawModel(&torus, depthProgram);
		drawModel(&floor, depthProgram);
		drawModel(&trashbin, depthProgram);

		glBindVertexArray(0);



		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);

		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);

		mat4 modelView = view * model;
		mat4 modelViewProjection = projection * view * model;

		//
		//mat4 depthBiasModelViewProjection = biasMatrix * depthModelViewProjection;
		mat4 depthBiasModelViewProjection = biasMatrix *dlit.getViewProjectionMatrix();
		//

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.getID());

		program.uniformMatrix4f(modelViewProjection, "modelViewProjection");
		program.uniformMatrix4f(modelView, "modelView");
		program.uniformLight(lit, "light[0]");
		program.uniform3f(ambient_model, "ambient_model");

		//
		program.uniformMatrix4f(depthBiasModelViewProjection, "depthBiasMVP");

		//program.uniform3f(lightDirection, "lit.direction");
		//program.uniform4f(lightColor, "lit.color");
		program.uniform3f((vec4(dlit.direction, 0) * modelView).xyz, "lit.direction");
		program.uniform4f(dlit.color, "lit.color");

		program.uniform1i(shadowResolution, "depth_resolution");
		//

		glActiveTexture(GL_TEXTURE1);
		glBindSampler(1, shadow_sampler);

		//glBindTexture(GL_TEXTURE_2D, depthTexture);
		glBindTexture(GL_TEXTURE_2D, dlit.getTextureID());
		program.uniform1i(1, "depth_texture");

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);

		glBindVertexArray(model_vao);

		drawModel(&torus, program);
		drawModel(&floor, program);
		drawModel(&trashbin, program);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}