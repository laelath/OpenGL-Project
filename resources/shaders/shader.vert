#version 330 core

#define MAX_DIRECTIONAL_LIGHTS 3

struct directional_light
{
	mat4 depthBiasMVP;
};

layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in vec3 in_Normal;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

out vec4 dLightShadowCoords[MAX_DIRECTIONAL_LIGHTS];

uniform mat4 modelView;
uniform mat4 modelViewProjection;

uniform directional_light directionalLights[MAX_DIRECTIONAL_LIGHTS];

void main()
{
	position = (modelView * vec4(in_Vertex, 1)).xyz;
	texCoord = in_TexCoord;
	normal = (modelView * vec4(in_Normal, 0)).xyz;

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		dLightShadowCoords[i] = directionalLights[i].depthBiasMVP * vec4(in_Vertex, 1);
	}
	
	gl_Position = modelViewProjection * vec4(in_Vertex, 1);
}