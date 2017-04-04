#version 400 core //compatibility

#include "structures.header"

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MMatrix;		//Model matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat3 NormalMatrixObj;
uniform mat4 TMatrix;		// texture projection matrix

uniform LightInfo light;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
out vec4 ecPosition;			
out vec3 ecLightDir;
out vec3 ecNormal;
out vec3 ecViewDir;
out float objNormalAngle;
out vec3 texCoords;

void main()
{
	ecPosition = MVMatrix * vec4(VertexPosition, 1.0f);
	ecLightDir = vec3((VMatrix * light.position) - ecPosition);
	ecNormal = NormalMatrix * VertexNormal;
	ecViewDir = -vec3(ecPosition);	// kamera je v (0, 0, 0)

	vec3 objNormal = NormalMatrixObj * VertexNormal;
	objNormalAngle = abs(dot(normalize(objNormal), vec3(0.0f, 0.0f, 1.0f)));
	
	texCoords = vec3(TMatrix * MMatrix * vec4(VertexPosition, 1.0f));

	gl_Position = PMatrix * ecPosition;
}
