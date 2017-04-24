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

	// light side
	if (lightAngle > 0.0f)
	{
		diffuse *= lightAngle;
		
		// specular
		vec3 toEye = reflect(-ecLightDirNormalized, ecNormalNormalized);
		float viewAngle = dot(toEye, ecNormalNormalized);
		if (viewAngle > 0.0f)
		{
			specular = (material.specular * light.specular) * pow(viewAngle, light.spotExponent);
		}
	}
	else diffuse = vec4(0.0f);

	FragColor = ambient + diffuse + specular;
}
