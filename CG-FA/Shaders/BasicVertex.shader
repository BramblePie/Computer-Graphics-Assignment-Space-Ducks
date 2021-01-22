#version 430 core

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;

//uniform vec3 camera;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform mat4 u_model;

out vec3 f_normal;
out vec2 f_uv;

void main()
{
	mat4 m = mat4(1.0);
	vec4 pos = u_model * vec4(v_pos, 1.0);
	gl_Position = u_projection * u_view * pos;

	f_normal = v_normal;
	f_uv = v_uv;
}