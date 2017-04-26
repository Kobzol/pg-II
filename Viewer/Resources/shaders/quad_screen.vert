#version 420 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTex;

out vec2 texCoords;
out vec2 centerVector;

void main()
{
	texCoords = VertexTex;
	centerVector = VertexPosition.xy;

	gl_Position = vec4(VertexPosition, 1.0);
}
