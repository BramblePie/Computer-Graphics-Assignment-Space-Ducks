#version 430 core

in vec3 f_color;

uniform vec3 u_color;

void main()
{
	gl_FragColor = vec4(f_color, 1.0);
}