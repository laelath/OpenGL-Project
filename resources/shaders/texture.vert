#version 330 core

layout(location = 0) in vec3 in_Vertex;

out vec2 texcoord;

void main()
{
	texcoord = (in_Vertex.xy + vec2(1,1)) * 0.5;
	gl_Position = vec4(in_Vertex, 1);
}