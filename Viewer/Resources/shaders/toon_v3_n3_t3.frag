#version 400 core //compatibility

#include "structures.header"

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D diffuseTexture;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec3 texCoords;

const float A = 0.1;
const float B = 0.3;
const float C = 0.6;
const float D = 1.0;

void main()
{
	vec3 ecLightDirNormalized = normalize(ecLightDir);
	vec3 ecNormalNormalized = normalize(ecNormal);
	vec3 ecViewDirNormalized = normalize(ecViewDir);

	vec4 textureColor = texture(diffuseTexture, texCoords.xy);

	vec4 ambient = material.ambient * light.ambient;
	vec4 diffuse = (textureColor * material.diffuse) * light.diffuse;
	vec4 specular = vec4(0.0f);

	float lightAngle = dot(ecLightDirNormalized, ecNormalNormalized);

	if (lightAngle < A) lightAngle = 0.0f;
	else if (lightAngle < B) lightAngle = B;
	else if (lightAngle < C) lightAngle = C;
	else if (lightAngle < D) lightAngle = D;

	// light side
	if (lightAngle > 0.0f)
	{
		diffuse *= lightAngle;
		
		// specular
		vec3 toEye = reflect(-ecLightDirNormalized, ecNormalNormalized);
		float viewAngle = dot(toEye, ecNormalNormalized);
		viewAngle = step(0.5f, viewAngle);

		if (viewAngle > 0.0f)
		{
			specular = (material.specular * light.specular) * pow(viewAngle, light.spotExponent);
		}
	}
	else diffuse = vec4(0.0f);

	float val = dot(ecViewDirNormalized, ecNormalNormalized);
	if (val < 0.15f)
	{
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else FragColor = ambient + diffuse + specular;
}
