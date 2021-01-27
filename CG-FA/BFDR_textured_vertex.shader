#version 430 core

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;

uniform vec3 camera;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

out vec3 f_normal;
out vec2 f_uv;

void main()
{
}