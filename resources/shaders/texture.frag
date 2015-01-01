#version 330 core

uniform sampler2D Texture;

in vec2 texcoord;

out vec4 frag_color;

void main()
{
	frag_color = texture(Texture, texcoord);
}