#version 330 core

layout(location = 0) in vec3 in_Vertex;

uniform mat4 depthMVP;

void main()
{
	gl_Position = depthMVP * vec4(in_Vertex, 1);
}