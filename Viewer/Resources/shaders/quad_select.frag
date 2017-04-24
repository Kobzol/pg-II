#version 420 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D diffuseTexture;

in vec2 texCoords;
in vec2 centerVector;

const float radius = 0.5f;

void main()
{
	float len = length(centerVector);

	if (len <= radius)
	{
		vec4 texColor = vec4(texture(diffuseTexture, texCoords).xyz, pow(len / radius, 2));
		FragColor = texColor;//vec4(0.0f, 1.0f, 0.0f, len / radius);
	}
	else discard;
}
