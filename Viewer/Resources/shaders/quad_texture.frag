#version 420 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D diffuseTexture;

in vec2 texCoords;
in vec2 centerVector;

void main()
{
	FragColor = texture(diffuseTexture, texCoords);
}
