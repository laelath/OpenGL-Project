#version 330 core

#define MAX_POINT_LIGHTS 1
#define MAX_DIRECTIONAL_LIGHTS 3

struct point_light
{
	mat4 depthMVP[6];
};

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

out vec4 plShadowCoords[MAX_POINT_LIGHTS];
out vec4 dlShadowCoords[MAX_DIRECTIONAL_LIGHTS];

uniform mat4 modelView;
uniform mat4 modelViewProjection;

uniform point_light Point_Lights[MAX_POINT_LIGHTS];
uniform directional_light Directional_Lights[MAX_DIRECTIONAL_LIGHTS];

void main()
{
	position = (modelView * vec4(in_Vertex, 1)).xyz;
	texCoord = in_TexCoord;
	normal = (modelView * vec4(in_Normal, 0)).xyz;

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		float axis = max(abs(in_Vertex.x), max(abs(in_Vertex.y), abs(in_Vertex.z)));
		if (axis == abs(in_Vertex.x))
		{
			if (axis == in_Vertex.x) plShadowCoords[i] = Point_Lights[i].depthMVP[0] * vec4(in_Vertex, 1);
			else plShadowCoords[i] = Point_Lights[i].depthMVP[1] * vec4(in_Vertex, 1);
		}
		else if (axis == abs(in_Vertex.y))
		{
			if (axis == in_Vertex.y) plShadowCoords[i] = Point_Lights[i].depthMVP[2] * vec4(in_Vertex, 1);
			else plShadowCoords[i] = Point_Lights[i].depthMVP[3] * vec4(in_Vertex, 1);
		}
		else if (axis == abs(in_Vertex.z))
		{
			if (axis == in_Vertex.z) plShadowCoords[i] = Point_Lights[i].depthMVP[4] * vec4(in_Vertex, 1);
			else plShadowCoords[i] = Point_Lights[i].depthMVP[5] * vec4(in_Vertex, 1);
		}
	}

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		dlShadowCoords[i] = Directional_Lights[i].depthBiasMVP * vec4(in_Vertex, 1);
	}
	
	gl_Position = modelViewProjection * vec4(in_Vertex, 1);
}