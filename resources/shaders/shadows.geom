#version 330 core

uniform mat4 shadowmapProjections[6];

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 18) out;

in vec3 pos[];

void main()
{
	for (int i = 1; i < 6; i++)
	{
		gl_Layer = i;
		for (int j = 0; j < 3; j++)
		{
			gl_Position = shadowmapProjections[i] * vec4(pos[j], 1);
			EmitVertex();
		}
		EndPrimitive();
	}
}