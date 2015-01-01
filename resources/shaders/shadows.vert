#version 330 core

layout(location = 0) in vec3 in_Vertex;

out vec3 vertex;

void main()
{
	vertex = in_Vertex;
}