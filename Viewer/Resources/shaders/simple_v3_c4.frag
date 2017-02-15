#version 400 core

in vec4 color;

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = color;
}
