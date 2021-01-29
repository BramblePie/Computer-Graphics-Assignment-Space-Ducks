#version 430 core

// Vertex attributes
in vec3 v_pos;
in vec3 v_normal;
in vec2 v_uv;
in vec3 v_tangent;
//in vec3 v_bitangent;

// MVP matrices
layout(location = 0) uniform mat4 u_view;
layout(location = 1) uniform mat4 u_projection;
uniform mat4 u_model;

layout(location = 2) uniform vec3 camera_pos;
uniform vec3 light_pos;

// All output data from vertex shader
out Vertex
{
	vec3 pos;
	vec2 uv;

	vec3 tan_pos;
	vec3 tan_camera;
	vec3 tan_light;
} v_out;

void main()
{
	vec4 pos = u_model * vec4(v_pos, 1.0);
	// Set vertex position and uv for fragment shader
	v_out.pos = pos.xyz;
	v_out.uv = v_uv;

	// Inverse model for normal and tangent transform
	mat3 inv_model = transpose(inverse(mat3(u_model)));
	vec3 T = normalize(inv_model * v_tangent);
	vec3 N = normalize(inv_model * v_normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	// Make TNB matrix to transform vectors to tangent space
	mat3 TBN = transpose(mat3(T, B, N));
	v_out.tan_light = TBN * light_pos;
	v_out.tan_camera = TBN * camera_pos;
	v_out.tan_pos = TBN * v_out.pos;

	gl_Position = u_projection * u_view * pos;
}