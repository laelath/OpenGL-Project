#version 330 core

layout(location = 0) in vec3 in_Vertex;

void main()
{
	gl_Position = vec4(in_Vertex, 1);
}