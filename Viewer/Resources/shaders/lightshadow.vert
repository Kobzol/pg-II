#version 420 core

layout (location = 0) in vec3 position;

uniform mat4 LightSpaceMatrix;
uniform mat4 MMatrix;

void main()
{
    gl_Position = LightSpaceMatrix * MMatrix * vec4(position, 1.0f);
}
