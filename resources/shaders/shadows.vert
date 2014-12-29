#version 330 core

uniform mat4 shadowmapProjections[6];

layout(location = 0) in vec3 in_Vertex;

out vec3 vertex;

void main()
{
	vertex = in_Vertex;
	gl_Position = shadowmapProjections[0] * vec4(in_Vertex, 1);
}