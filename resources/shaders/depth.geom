#version 330 core

uniform mat4 modelViewProjections[6];
uniform int layers;

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 18) out;

void main()
{
	for (int i = 0; i < layers; i++)
	{
		gl_Layer = i;
		for (int j = 0; j < 3; j++)
		{
			gl_Position = modelViewProjections[i] * gl_in[j].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}