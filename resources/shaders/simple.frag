#version 330 core

struct material
{
	sampler2D texture;
};

in vec2 texCoord;
in vec3 normal;

in vec3 shadowCoord;

out vec4 frag_color;

uniform material mat;
uniform vec3 ambient_model;

uniform vec3 light_color;
uniform vec3 light_direction;
uniform sampler2D shadowMap;

uniform mat4 modelView;

void main()
{
	frag_color = vec4(0,0,0,1);
	if (texture(shadowMap, shadowCoord.xy).z > shadowCoord.z)
	{
		frag_color = texture(mat.texture, texCoord);
		vec3 direction = (modelView * vec4(light_direction, 0)).xyz;
		frag_color.rgb *= light_color * max(0.0, dot(normal, normalize(direction))) + ambient_model;
	}
}