#version 430 core

const float PI = 3.1415926535897932384626433832795;

in Vertex
{
	// World space
	vec3 position;
	vec2 uv;
	vec3 normal;
	
	// // Tangent space transform
	// mat3 TBN;
	
	vec3 tan_pos;
	vec3 tan_camera;
	vec3 tan_light;
} f_in;

struct Material
{
	vec3 color;
	float metallic;
	float roughness;
};

uniform Material material;

struct Light
{
	// World space 
	vec3 color;
	vec3 position;
};

uniform Light lights[8];

// World space
layout(location = 2) uniform vec3 camera_pos;

// Diffuse color texture
uniform sampler2D tex_diffuse;

out vec4 color;

float ThrowbridgeReitzGGX(vec3 N, vec3 H, float a);

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k);

vec3 FresnelSchlick(float HV, vec3 F0);

void main()
{
	// Interpolated normal vector from vertex shader
	vec3 normal = normalize(f_in.normal);
	// View direction
	vec3 view = normalize(camera_pos - f_in.position);

	vec3 albedo = texture(tex_diffuse, f_in.uv).rgb;

	vec3 F0 = mix(vec3(0.04), albedo, material.metallic);

	// Roughness equations
	float a = material.roughness * material.roughness;
	float k = (a + 1.0) * (a + 1.0) / 8.0;

	// Total illumination 
	vec3 lum = vec3(0.0);
	for(int i = 0; i < 8; i++)
	{
		// Incoming light direction on framgent position
		vec3 L = normalize(lights[i].position - f_in.position);
		// halfway vector
		vec3 halfway = normalize(L + view);

		// Normal Distribution Function (NDF) or specular/normal distribution
        float D = ThrowbridgeReitzGGX(normal, halfway, a); 

		// Geometry function
        float G = GeometrySmith(normal, view, L, k);   

		// Fresnel equation
        vec3 F = FresnelSchlick(clamp(dot(halfway, view), 0.0, 1.0), F0);

		// Cook-Torrance specular
        float denominator = 4 * max(dot(normal, view), 0.0) * max(dot(normal, L), 0.0);
        vec3 CT = D * F * G / max(denominator, 0.001);

		vec3 kD = vec3(1.0) - F;
        kD *= 1.0 - material.metallic;

		// Distance between light and fragment position
		float d = length(lights[i].position - f_in.position);
		float attenuation = 1.0 / (d * d);

		// Scaled by angle of incident
		float NL = max(dot(normal, L), 0.0); 

		// All illumination by this single light on fragment
		lum += (kD * albedo / PI + CT) * attenuation * lights[i].color * NL;
	}

	// Adding ambient light
	color.rgb = vec3(0.005) * albedo + lum;
	color.a = 1.0;

	// Tone mapping and gamma
	color.rgb = color.rgb / (color.rgb + vec3(1.0));
	color.rgb = pow(color.rgb, vec3(1.0/2.2));
}

vec3 FresnelSchlick(float HV, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - HV, 0.0), 5.0);
}

float GeometrySchlickGGX(float NV, float k)
{
    return NV / (NV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	// k = roughness^2 / 2
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
	
    return GeometrySchlickGGX(NdotV, k) * GeometrySchlickGGX(NdotL, k);
}

float ThrowbridgeReitzGGX(vec3 N, vec3 H, float a)
{
	// a^2
	float a2 = a*a;
	// N · H
    float NdotH = max(dot(N, H), 0.0);
	// (N · H)^2
    float NdotH2 = NdotH*NdotH;
	// (N · H)^2 * (a^2 - 1) + 1
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
	// PI * ((N · H)^2 * (a^2 - 1) + 1)^2
    denom = PI * denom * denom;
	// a^2 / (PI * ((N · H)^2 * (a^2 - 1) + 1)^2)
    return a2 / denom;
}