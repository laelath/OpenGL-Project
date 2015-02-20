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
	bool textured;
};

#define MAX_POINT_LIGHTS 1
#define MAX_DIRECTIONAL_LIGHTS 3
#define MAX_SPOT_LIGHTS 1

uniform material mat;

uniform point_light Point_Lights[MAX_POINT_LIGHTS];
uniform directional_light Directional_Lights[MAX_DIRECTIONAL_LIGHTS];
uniform spot_light Spot_Lights[MAX_SPOT_LIGHTS];

uniform int Depth_Resolution;

uniform vec3 ambient_model;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

in vec4 dLightShadowCoords[MAX_DIRECTIONAL_LIGHTS];

layout(location = 0) out vec4 frag_color;

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

	/*for (int i = 0; (i < MAX_POINT_LIGHTS) && (Point_Lights[i].color.a != 0); i++)
	{
		vec3 light_direction = normalize(Point_Lights[i].position - position);
		vec3 light_diffuse = diffuseLighting(Point_Lights[i], light_direction);
		diffuse_level += light_diffuse;
		
		if (light_diffuse.r > 0.0 || light_diffuse.g > 0.0 || light_diffuse.b > 0.0)
		{
			specular_level += specularLighting(Point_Lights[i], light_direction, view_direction);
		}
	}*/

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		float cosTheta = clamp(dot(normal, Directional_Lights[i].direction), 0, 1);
		float bias = clamp(10 * tan(acos(cosTheta)) / Depth_Resolution, 0.0, 0.01);

		float visibility = texture(Directional_Lights[i].depth_texture, 
				vec3(dLightShadowCoords[i].xy, (dLightShadowCoords[i].z - bias) / dLightShadowCoords[i].w));

		/*float visibility;
		if (i == 0)
		{
			visibility = texture(Directional_Lights[0].depth_texture, 
				vec3(dLightShadowCoords[i].xy, (dLightShadowCoords[i].z - bias) / dLightShadowCoords[i].w));
		}
		else if (i == 1)
		{
			visibility = texture(Directional_Lights[1].depth_texture, 
				vec3(dLightShadowCoords[i].xy, (dLightShadowCoords[i].z - bias) / dLightShadowCoords[i].w));
		}
		else if (i == 2)
		{
			visibility = texture(Directional_Lights[2].depth_texture, 
				vec3(dLightShadowCoords[i].xy, (dLightShadowCoords[i].z - bias) / dLightShadowCoords[i].w));
		}*/

		if (visibility != 0.0)
		{
			vec3 light_diffuse = diffuseLighting(Directional_Lights[i]);
			
			if (light_diffuse.r > 0.0 || light_diffuse.g > 0.0 || light_diffuse.b > 0.0)
			{
				diffuse_level += light_diffuse;
				specular_level += specularLighting(Directional_Lights[i], view_direction);
			}
		}
	}

	/*for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		vec3 light_direction = normalize(Spot_Lights[i].position - position);
		vec3 light_diffuse = diffuseLighting(Spot_Lights[i], light_direction);
		diffuse_level += light_diffuse;

		if (light_diffuse.r > 0.0 || light_diffuse.g > 0.0 || light_diffuse.b > 0.0)
		{
			specular_level += specularLighting(Spot_Lights[i], light_direction, view_direction);
		}
	}*/

	vec3 ambient_level = ambient_model.rgb * mat.ambient;
	diffuse_level *= mat.diffuse;

	float transparency = mat.opacity;

	if (mat.textured)
	{
		vec4 diffuse_texture = texture(mat.texture, texCoord);
		ambient_level *= diffuse_texture.rgb;
		diffuse_level *= diffuse_texture.rgb;
		transparency *= diffuse_texture.a;
	}

	specular_level *= mat.specular /* mat.shine_strength*/;
	
	frag_color.rgb = diffuse_level + specular_level + ambient_level;
	frag_color.a = transparency;
}