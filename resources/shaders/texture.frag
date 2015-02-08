#version 330 core

uniform sampler2D Texture;

in vec2 texcoord;

layout(location = 0) out vec4 frag_color;

void main()
{
	frag_color.rgb = vec3(texture(Texture, texcoord).r);
	frag_color.a = 1.0;
}