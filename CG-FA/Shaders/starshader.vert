# version 430 core

// Vertex attributes
in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;
in vec3 v_tangent;

// MVP matrices
layout(location = 0) uniform mat4 u_view;
layout(location = 1) uniform mat4 u_projection;
layout(location = 2) uniform vec3 camera_pos;
uniform mat4 u_model;

// All output data from vertex shader
out Vertex
{
	vec3 position;
	vec3 normal;
	vec3 up;
	vec3 right;
} v_out;

void main()
{
	vec4 pos = u_model * vec4(v_pos, 1.0);
	// Set vertex position and uv for fragment shader
	v_out.position = pos.xyz;
	v_out.normal = normalize(mat3(transpose(inverse(u_model))) * v_normal);
	v_out.up = vec3(u_view[0][0], u_view[1][0], u_view[2][0]);
	v_out.right = vec3(u_view[0][1], u_view[1][1], u_view[2][1]);

	gl_Position = u_projection * u_view * pos;
}