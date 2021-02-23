# version 430 core

in Vertex
{
	vec3 position;
	vec2 uv;
	vec3 normal;

	// Tangent space transform
	mat3 TBN;
} f_in;

uniform vec3 u_color;
uniform sampler2D tex_diffuse;

void main()
{
	//gl_FragColor = vec4(u_color, 1.0);
	//gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	gl_FragColor = texture(tex_diffuse, f_in.uv);
}