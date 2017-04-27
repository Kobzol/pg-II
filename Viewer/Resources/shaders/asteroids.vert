#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 PMatrix;
uniform mat4 VMatrix;

void main()
{
    gl_Position = PMatrix * VMatrix * instanceMatrix * vec4(position, 1.0f); 
    TexCoords = texCoords.xy;
}
