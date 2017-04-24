#version 400 core //compatibility

#include "structures.header"

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float Time;
			
in vec3 ecLightDir[];
in vec3 ecNormal[];
in vec3 ecViewDir[];
in vec3 texCoords[];

out vec3 geomLightDir;
out vec3 geomNormal;
out vec3 geomViewDir;
out vec3 geomTexCoords;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}
vec4 Explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(Time) + 1.0f) / 2.0f) * magnitude; 
    return position + vec4(direction, 0.0f);
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = Explode(gl_in[0].gl_Position, normal);
    geomLightDir = ecLightDir[0];
	geomNormal = ecNormal[0];
	geomViewDir = ecLightDir[0];
	geomTexCoords = texCoords[0];
    EmitVertex();

    gl_Position = Explode(gl_in[1].gl_Position, normal);
    geomLightDir = ecLightDir[1];
	geomNormal = ecNormal[1];
	geomViewDir = ecLightDir[1];
	geomTexCoords = texCoords[1];
    EmitVertex();

    gl_Position = Explode(gl_in[2].gl_Position, normal);
    geomLightDir = ecLightDir[2];
	geomNormal = ecNormal[2];
	geomViewDir = ecLightDir[2];
	geomTexCoords = texCoords[2];
    EmitVertex();

    EndPrimitive();
}  