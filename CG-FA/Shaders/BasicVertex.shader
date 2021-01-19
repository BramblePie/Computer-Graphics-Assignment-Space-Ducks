#version 430 core

in vec3 v_pos;
in vec3 v_color;

//uniform vec3 camera;
uniform mat4 view;
uniform mat4 projection;

out vec3 f_color;

void main()
{
	gl_Position = projection * view * vec4(v_pos, 1.0);
	f_color = v_color;
}