#version 430 core

in Vertex
{
	// World space
	vec3 position;
	vec3 normal;
} f_in;


struct Material
{
	vec3 color;
	float metallic;
	float roughness;
};

uniform Material material;
layout(location = 2) uniform vec3 camera_pos;

out vec4 color;

void main()
{
	const vec3 view = normalize(camera_pos - f_in.position);
    const float a = max(dot(view, f_in.normal), .0);
    color.rgb = mix(material.color, vec3(1.2), pow(a, 32));
    color.a = pow(a, 16);
	// color.a = 1.0;
}