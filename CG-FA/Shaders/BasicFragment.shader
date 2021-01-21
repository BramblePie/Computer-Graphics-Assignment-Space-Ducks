#version 430 core

//in vec3 f_color;
in vec3 f_normal;
in vec2 f_uv;

layout(location = 1) uniform vec3 u_color;

void main()
{
	gl_FragColor = vec4(u_color, 1.0);
}