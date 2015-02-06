#version 330 core

struct point_light
{
	vec3 position;
	vec4 color;

	//samplerCubeShadow depth_texture;
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

	//sampler2DShadow depth_texture;
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

#define MAX_POINT_LIGHTS 1
#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_SPOT_LIGHTS 1

uniform material mat;

uniform point_light pointLights[MAX_POINT_LIGHTS];
uniform directional_light directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform spot_light spotLights[MAX_SPOT_LIGHTS];

uniform int depth_resolution;

uniform vec3 ambient_model;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

in vec4 dLightShadowCoords[MAX_DIRECTIONAL_LIGHTS];

out vec4 frag_color;

vec3 diffuseLighting(const point_light light, vec3 light_direction)
{
	float light_distance = distance(position, light.position);
	float distance_intensity = light.color.a * pow(light_distance, -1.0);
	float diffuse_intensity = max(0.0, dot(normal, light_direction)) * distance_intensity;
	return light.color.rgb * diffuse_intensity;
}

vec3 diffuseLighting(const directional_light light)
{
	float diffuse_intensity = max(0.0, dot(normal, light.direction));
	return light.color.rgb * diffuse_intensity;
}

vec3 diffuseLighting(const spot_light light, vec3 light_direction)
{
	float cosValue = max(0.0, dot(light.direction, light_direction));
	if (cosValue >= light.cosLimit)
	{
		float light_distance = distance(position, light.position);
		float distance_intensity = light.color.a * pow(light_distance, -1.0);
		float diffuse_intensity = max(0.0, dot(normal, light_direction)) * distance_intensity * cosValue;
		return light.color.rgb * diffuse_intensity;
	}
	return vec3(0);
}

vec3 specularLighting(const point_light light, vec3 light_direction, vec3 view_direction)
{
	vec3 reflection_direction = normalize(reflect(light_direction, normal));
	float specular = max(0.0, dot(view_direction, reflection_direction));
	float fspecular = pow(specular, mat.shininess);
	return light.color.rgb * fspecular;
}

vec3 specularLighting(const directional_light light, vec3 view_direction)
{
	vec3 reflection_direction = normalize(reflect(light.direction, normal));
	float specular = max(0.0, dot(view_direction, reflection_direction));
	float fspecular = pow(specular, mat.shininess);
	return light.color.rgb * fspecular;
}

vec3 specularLighting(const spot_light light, vec3 light_direction, vec3 view_direction)
{
	vec3 reflection_direction = normalize(reflect(light_direction, normal));
	float specular = max(0.0, dot(view_direction, reflection_direction));
	float fspecular = pow(specular, mat.shininess);
	return light.color.rgb * fspecular;
}

void main()
{
	vec3 diffuse_level = vec3(0,0,0);
	vec3 specular_level = vec3(0,0,0);
	vec3 view_direction = normalize(position);

	for (int i = 0; (i < MAX_POINT_LIGHTS) && (pointLights[i].color.a != 0); i++)
	{
		vec3 light_direction = normalize(pointLights[i].position - position);
		vec3 light_diffuse = diffuseLighting(pointLights[i], light_direction);
		diffuse_level += light_diffuse;
		
		if (light_diffuse.r > 0.0 || light_diffuse.g > 0.0 || light_diffuse.b > 0.0)
		{
			specular_level += specularLighting(pointLights[i], light_direction, view_direction);
		}
	}

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		float cosTheta = clamp(dot(normal, directionalLights[i].direction), 0, 1);
		float bias = clamp(10 * tan(acos(cosTheta)) / depth_resolution, 0.0, 0.01);

		float visibility = texture(directionalLights[i].depth_texture, 
				vec3(dLightShadowCoords[i].xy, (dLightShadowCoords[i].z - bias) / dLightShadowCoords[i].w));

		if (visibility != 0.0)
		{
			vec3 light_diffuse = diffuseLighting(directionalLights[i]);
			diffuse_level += light_diffuse;
			
			if (light_diffuse.r > 0.0 || light_diffuse.g > 0.0 || light_diffuse.b > 0.0)
			{
				specular_level += specularLighting(directionalLights[i], view_direction);
			}
		}
	}

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		vec3 light_direction = normalize(spotLights[i].position - position);
		vec3 light_diffuse = diffuseLighting(spotLights[i], light_direction);
		diffuse_level += light_diffuse;

		if (light_diffuse.r > 0.0 || light_diffuse.g > 0.0 || light_diffuse.b > 0.0)
		{
			specular_level += specularLighting(spotLights[i], light_direction, view_direction);
		}
	}

	vec4 diffuse_texture = texture(mat.texture, texCoord);
	vec3 ambient_level = ambient_model.rgb * diffuse_texture.rgb * mat.ambient;
	diffuse_level *= diffuse_texture.rgb * mat.diffuse;
	specular_level *= mat.specular /* mat.shine_strength*/;
	
	frag_color.rgb = diffuse_level + specular_level + ambient_level;
	frag_color.a = diffuse_texture.a * mat.opacity;
}