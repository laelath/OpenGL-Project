#version 330 core

layout(location = 0) in vec3 in_Vertex;

uniform mat4 modelViewProjection;

void main()
{
	gl_Position = modelViewProjection * vec4(in_Vertex, 1);
}