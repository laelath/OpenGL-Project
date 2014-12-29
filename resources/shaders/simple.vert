#version 330 core

layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

out vec2 texCoord;
out vec3 normal;

out vec3 shadowCoord;

uniform mat4 depthBiasMVP;

uniform mat4 modelView;
uniform mat4 modelViewProjection;

void main()
{
	texCoord = in_TexCoord;
	normal = (modelView * vec4(in_Normal, 0)).xyz;
	
	shadowCoord = (depthBiasMVP * vec4(in_Vertex, 1)).xyz;
	
	gl_Position = modelViewProjection * vec4(in_Vertex, 1);
}