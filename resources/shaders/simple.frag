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
uniform sampler2DShadow depth_texture;
uniform int depth_resolution;

uniform vec3 ambient_model;

uniform mat4 modelView;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

in vec4 shadowCoord;

layout(location = 0) out vec4 frag_color;

const vec2 poissonDisk[16] = vec2[](
	vec2(-0.949914f, 0.02824331f),
	vec2(-0.3765172f, -0.188862f),
	vec2(-0.6290753f, -0.6441603f),
	vec2(-0.8430508f, 0.5020682f),
	vec2(-0.4366184f, 0.3660368f),
	vec2(0.1400093f, 0.3042954f),
	vec2(0.07860211f, -0.167511f),
	vec2(0.009609296f, 0.7620118f),
	vec2(-0.5071639f, 0.7898663f),
	vec2(-0.232526f, -0.7719541f),
	vec2(0.1843012f, -0.7290095f),
	vec2(0.6692578f, 0.3599324f),
	vec2(0.6320484f, -0.4929866f),
	vec2(0.5308439f, -0.05103759f),
	vec2(0.4338843f, 0.7917479f),
	vec2(0.95281f, 0.02346634f)
);

const vec2 edgeTestDisk[8] = vec2[](
	vec2(-1.0, -1.0),
	vec2( 1.0, -1.0),
	vec2(-1.0,  1.0),
	vec2( 1.0,  1.0),
	vec2( 1.414, 0.0),
	vec2(-1.414, 0.0),
	vec2(0.0,  1.414),
	vec2(0.0, -1.414)
);

void main()
{
	vec4 texture_color = texture(mat.texture, texCoord);
	vec3 ambient_color = texture_color.rgb * ambient_model;
	vec3 diffuse_color = vec3(0);
	vec3 specular_color = vec3(0);
	
	//vec3 light_direction = (modelView * vec4(lit.direction, 0)).xyz;
	
	float cosTheta = clamp(dot(normal, lit.direction), 0, 1);
	float bias = clamp(10 * tan(acos(cosTheta)) / depth_resolution, 0.0, 0.01);
	float visibility = 0;

	for (int i = 0; i < 8; i++)
	{
		visibility += 0.125 * texture(depth_texture, vec3(shadowCoord.xy + 2.5 * edgeTestDisk[i] / depth_resolution, (shadowCoord.z - bias) / shadowCoord.w));
	}

	if (visibility != 1.0 && visibility != 0.0)
	{
		visibility = 0.0;
		for (int i = 0; i < 16; i++)
		{
			visibility += 0.0625 * texture(depth_texture, vec3(shadowCoord.xy + 2.5 * poissonDisk[i] / depth_resolution, (shadowCoord.z - bias) / shadowCoord.w));
		}
	}
	
	if (visibility != 0.0)
	{
		diffuse_color = texture_color.rgb * lit.color.rgb * max(0.0, dot(normal, lit.direction));
		
		vec3 view_direction = normalize(position);
		vec3 reflection_direction = normalize(reflect(lit.direction, normal));
		float specular = max(0.0, dot(view_direction, reflection_direction));
		float fspecular = pow(specular, 25);
		specular_color = lit.color.rgb * fspecular;
	}
	
	frag_color.rgb = ambient_color + visibility * (diffuse_color + specular_color);
	frag_color.a = texture_color.a;
}