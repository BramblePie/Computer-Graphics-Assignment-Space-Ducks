#version 430 core

in vec3 texCoords;

layout (location = 3) uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    FragColor = texture(skybox, texCoords);
    // FragColor = vec4(0.0);
}