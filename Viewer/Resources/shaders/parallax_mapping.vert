#version 400 core //compatibility

#include "structures.header"

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;
layout(location = 3) in vec3 VertexTBNTangent;
layout(location = 4) in vec3 VertexTBNBitangent;
layout(location = 5) in vec3 VertexTBNNormal;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)

uniform LightInfo light;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
out VS_OUT
{
	vec3 ecLightDir;
	vec3 ecNormal;
	vec3 ecViewDir;
	vec2 texCoords;
} vs_out;

void main()
{
	vec4 ecPosition = MVMatrix * vec4(VertexPosition, 1.0f);
	vec3 ecLightDir = vec3((VMatrix * light.position) - ecPosition);
	vec3 ecNormal = NormalMatrix * VertexNormal;
	vec3 ecViewDir = -vec3(ecPosition);	// kamera je v (0, 0, 0)

	vec3 T = vec3(MVMatrix * vec4(VertexTBNTangent, 0.0f));
	vec3 B = vec3(MVMatrix * vec4(VertexTBNBitangent, 0.0f));
	vec3 N = vec3(MVMatrix * vec4(VertexTBNNormal, 0.0f));
	
	vs_out.ecNormal = vec3(dot(T, ecNormal), dot(B, ecNormal), dot(N, ecNormal));			// dot optimizes TBN matrix multiplication
	vs_out.ecLightDir = vec3(dot(T, ecLightDir), dot(B, ecLightDir), dot(N, ecLightDir));
	vs_out.ecViewDir = vec3(dot(T, ecViewDir), dot(B, ecViewDir), dot(N, ecViewDir));
	vs_out.texCoords = VertexTex.xy;

	gl_Position = PMatrix * ecPosition;
}
