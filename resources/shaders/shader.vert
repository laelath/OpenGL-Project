#version 330 core

layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

uniform mat4 modelView;
uniform mat4 modelViewProjection;

void main()
{
	position = (modelView * vec4(in_Vertex, 1)).xyz;
	texCoord = in_TexCoord;
	normal = (modelView * vec4(in_Normal, 0)).xyz;
	
	gl_Position = modelViewProjection * vec4(in_Vertex, 1);
}