#version 430 core

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;
//in vec3 v_color;

//uniform vec3 camera;
uniform mat4 view;
uniform mat4 projection;

//out vec3 f_color;
out vec3 f_normal;
out vec2 f_uv;

void main()
{
	gl_Position = projection * view * vec4(v_pos, 1.0);
	//f_color = v_color;
	f_normal = v_normal;
	f_uv = v_uv;
}