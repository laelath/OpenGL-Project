#version 330 core

struct material
{
	sampler2D texture;
};

struct direc_light
{
	vec3 direction;
	vec4 color;
};

uniform material mat;
uniform direc_light lit;
uniform vec3 ambient_model;

uniform mat4 modelView;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 frag_color;

void main()
{
	vec4 texture_color = texture(mat.texture, texCoord);
	vec3 ambient_color = texture_color.rgb * ambient_model;
	vec3 light_direction = (modelView * vec4(lit.direction, 0)).xyz;
	vec3 diffuse_color = texture_color.rgb * lit.color.rgb * max(0.0, dot(normal, light_direction));
	
	vec3 view_direction = normalize(position);
	vec3 reflection_direction = normalize(reflect(light_direction, normal));
	float specular = max(0.0, dot(view_direction, reflection_direction));
	float fspecular = pow(specular, 25);
	vec3 specular_color = lit.color.rgb * fspecular;
	
	frag_color.rgb = ambient_color + diffuse_color + specular_color;
	frag_color.a = texture_color.a;
}