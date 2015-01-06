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
uniform vec3 ambient_model;

uniform mat4 modelView;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

in vec4 shadowCoord;

out vec4 frag_color;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

void main()
{
	vec4 texture_color = texture(mat.texture, texCoord);
	vec3 ambient_color = texture_color.rgb * ambient_model;
	vec3 diffuse_color = vec3(0);
	vec3 specular_color = vec3(0);
	
	vec3 light_direction = (modelView * vec4(lit.direction, 0)).xyz;
	
	float cosTheta = clamp(dot(normal, light_direction), 0, 1);
	float bias = clamp(tan(acos(cosTheta)), 0.002, 0.05);
	//float bias = 0.005;
	float visibility = 0;
	for (int i = 0; i < 16; i++)
	{
		visibility += 0.0625 * texture(depth_texture, vec3(shadowCoord.xy + poissonDisk[i] / 2000.0, (shadowCoord.z - bias) / shadowCoord.w));
	}
	
	if (visibility != 0.0)
	{
		diffuse_color = texture_color.rgb * lit.color.rgb * max(0.0, dot(normal, light_direction));
		
		vec3 view_direction = normalize(position);
		vec3 reflection_direction = normalize(reflect(light_direction, normal));
		float specular = max(0.0, dot(view_direction, reflection_direction));
		float fspecular = pow(specular, 25);
		specular_color = lit.color.rgb * fspecular;
	}
	
	frag_color.rgb = ambient_color + visibility * (diffuse_color + specular_color);
	frag_color.a = texture_color.a;
}