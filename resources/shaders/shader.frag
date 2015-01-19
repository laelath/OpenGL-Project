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

struct spot_light
{
	vec3 position;
	vec3 direction;
	vec4 color;
	
	float cosLimit;

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

const int max_point_lights = 1;
const int max_directional_lights = 1;
const int max_spot_lights = 0;

uniform material mat;

uniform point_light pointLight[max_point_lights];
uniform directional_light directionalLight[max_directional_lights];
uniform spot_light spotLight[max_spot_lights]

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
	
	for (int i = 0; (i < max_point_lights) && (pointLight[i].color.a != 0); i++)
	{
		vec3 light_position = (modelView * vec4(pointLight[i].position, 1)).xyz;
		vec3 light_direction = normalize(light_position - position);
		float light_distance = distance(position, light_position);
		float distance_intensity = pointLight[i].color.a * pow(light_distance, -1.0);
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

vec3 diffuseLighting(const point_light light)
{
	vec3 light_position = (modelView * vec4(light.position, 1)).xyz;
	vec3 light_direction = normalize(light_position - position);
	float light_distance = distance(position, light_position);
	float distance_intensity = light.color.a * pow(light_distance, -1.0);
	float diffuse_intensity = max(0.0, dot(normal, light_direction)) * distance_intensity;
	return diffuse_intensity * light.color.rgb;
}

vec3 diffuseLighting(const directional_light light)
{
	
}

vec3 diffuseLighting(const spot_light light)
{
	
}

vec3 specularLighting(const point_light light, vec3 view_direction)
{
	
}

vec3 specularLighting(const directional_light light, vec3 view_direction)
{
	
}

vec3 specularLighting(const spot_light light, vec3 view_direction)
{
	
}