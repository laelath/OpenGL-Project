#version 330 core

struct point_light
{
	vec3 position;
	vec4 color;
};

struct directional_light
{
	vec3 direction;
	vec4 color;

	sampler2DShadow depth_texture;
};

struct material
{
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
	float opacity;
	float shininess;
	float shine_strength;
	sampler2D texture;
};

const int max_lights = 1;

uniform material mat;

uniform point_light plight[max_lights];
uniform directional_light dlight[max_lights];

uniform vec3 ambient_model;

uniform mat4 modelView;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

in vec4 shadowCoord;

out vec4 frag_color;

void main()
{
	vec3 diffuse_level = vec3(0,0,0);
	vec3 specular_level = vec3(0,0,0);
	vec3 view_direction = normalize(position);
	
	for (int i = 0; (i < max_lights) && (plight[i].color.a != 0); i++)
	{
		vec3 light_position = (modelView * vec4(plight[i].position, 1)).xyz;
		vec3 light_direction = normalize(light_position - position);
		float light_distance = distance(position, light_position);
		float distance_intensity = plight[i].color.a / light_distance /*pow(light_distance, 2.0)*/;
		float diffuse_intensity = max(0.0, dot(normal, light_direction)) * distance_intensity;
		diffuse_level += diffuse_intensity * light[i].color.rgb;
		
		if (diffuse_intensity > 0.0)
		{
			vec3 reflection_direction = normalize(reflect(light_direction, normal));
			float specular = max(0.0, dot(view_direction, reflection_direction));
			float fspecular = pow(specular, mat.shininess);
			specular_level += fspecular * light[i].color.rgb;
		}
	}

	vec4 diffuse_texture = texture(mat.texture, texCoord);
	vec3 ambient_level = ambient_model.rgb * diffuse_texture.rgb * mat.ambient;
	diffuse_level *= diffuse_texture.rgb * mat.diffuse;
	specular_level *= mat.specular /* mat.shine_strength*/;
	
	frag_color.rgb = diffuse_level + specular_level + ambient_level;
	frag_color.a = diffuse_texture.a * mat.opacity;
}

vec3 calculateDiffuseLighting(point_light light)
{
	
}

vec3 calculateSpecularLighting(point_light light)
{
	
}

vec3 calculateDiffuseLighting(directional_light light)
{
	
}

vec3 calculateSpecularLighting(directional_light light)
{
	
}