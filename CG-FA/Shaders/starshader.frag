#version 430 core

in Vertex
{
	// World space
	vec3 position;
	vec3 normal;
	vec3 up;
	vec3 right;
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

	const vec3 burst = normalize(cross(f_in.normal, view));

	const float f = pow(1.0 - (2.0 * abs(dot(burst, f_in.right)) * abs(dot(burst, f_in.up))), 4);

    color.rgb = mix(material.color, vec3(1.2), pow(a, 32));
	const float d = pow(a, 16);
    color.a = d * (d + f * (1.0 - d));
}