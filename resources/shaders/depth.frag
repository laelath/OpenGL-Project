#version 330 core

layout(location = 0) out vec4 frag_depth;

void main()
{
	frag_depth.rgb = vec3(gl_FragCoord.z);
	frag_depth.a = 1.0;
}