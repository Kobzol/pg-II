#version 400 core //compatibility

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;

uniform mat4 MMatrix;
uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrixObj;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)

//The prefix wc means WORLD SPACE
out vec3 wcPosition;
out vec3 wcNormal;
out vec2 wcTexCoords;

void main()
{
	//TODO: Update OUT variables

	wcPosition = vec3(MMatrix * vec4(VertexPosition, 1.0f));
	wcNormal = NormalMatrixObj * VertexNormal; //vec3(MMatrix * vec4(VertexNormal, 0.0f));
	wcTexCoords = VertexTex.xy;
}
