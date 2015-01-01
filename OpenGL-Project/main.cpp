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

vec3 ambient_model = vec3(0.01f, 0.05f, 0.2f);
//vec3 ambient_model = vec3(0.1f, 0.5f, 1.0f);

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
	glCullFace(GL_BACK);
	glClearColor(ambient_model.r, ambient_model.g, ambient_model.b, 0.1f);

	Shader program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");
	//TESTING SHADER
	Shader simple("../resources/shaders/simple.vert", "../resources/shaders/simple.frag");
	Shader texturepass("../resources/shaders/texture.vert", "../resources/shaders/texture.frag");
	//

	GLuint sampler;
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	/*GLuint shadow_sampler;
	glGenSamplers(1, &shadow_sampler);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(shadow_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

	model torus, trashbin, floor;
	load3DFromFile("../resources/models/torusball/torusball.obj", &torus);
	load3DFromFile("../resources/models/trashcan/trashbin.obj", &trashbin);
	load3DFromFile("../resources/models/floor/floor.obj", &floor);

	light lit;
	lit.position = vec3(-20, 100, 0);
	lit.color = vec4(1.0, 0.9, 0.7, 15000);

	//TEMP CODE

	GLuint test_texture = loadTexture2D("../resources/textures/testtexture.tga");

	float quad_vertex_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	
	/*GLuint quad_vertex_array, quad_vertex_buffer;

	glGenVertexArrays(1, &quad_vertex_array);
	glBindVertexArray(quad_vertex_array);

	glGenBuffers(1, &quad_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_data), quad_vertex_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	//END OF TEMP CODE

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		glViewport(0, 0, width, height);

		computeMatrices(window);
		mat4 projection = getProjectionMatrix();
		mat4 view = getViewMatrix();
		mat4 model = mat4(1.0f);

		mat4 modelView = view * model;
		mat4 modelViewProjection = projection * view * model;

		vec4 debug = modelViewProjection * vec4(-40, 20, 10, 1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.id);

		program.uniformMatrix4f(modelViewProjection, "modelViewProjection");
		program.uniformMatrix4f(modelView, "modelView");
		program.uniformLight(lit, "light[0]");
		program.uniform3f(ambient_model, "ambient_model");

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);

		drawModel(&torus, program);
		drawModel(&floor, program);
		drawModel(&trashbin, program);

		/*/
		glViewport(0, 0, 256, 256);

		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(texturepass.id);

		glActiveTexture(GL_TEXTURE0);
		glBindSampler(0, sampler);
		
		glBindTexture(GL_TEXTURE_2D, test_texture);
		texturepass.uniform1i(0, "Texture");

		glEnableVertexAttribArray(0);

		glBindVertexArray(quad_vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_buffer);
		
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		//*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}