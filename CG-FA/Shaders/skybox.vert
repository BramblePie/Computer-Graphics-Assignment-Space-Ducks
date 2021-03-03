# version 430 core

in vec3 pos;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 texCoords;

void main()
{
    texCoords = pos;
    gl_Position = u_projection * u_view * vec4(pos, 1.0);
}