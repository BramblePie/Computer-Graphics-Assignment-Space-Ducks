#version 430 core

//in vec3 f_color;
in vec3 f_normal;
in vec2 f_uv;

uniform vec3 u_color;
uniform sampler2D diff_tex;

void main()
{
	//gl_FragColor = vec4(u_color, 1.0);
	//gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
	gl_FragColor = texture(diff_tex, f_uv);
}