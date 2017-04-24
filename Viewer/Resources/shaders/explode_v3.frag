#version 400 core //compatibility

#include "structures.header"

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D diffuseTexture;

//The prefix ec means Eye Coordinates in the Eye Coordinate System	
in vec3 geomLightDir;
in vec3 geomNormal;
in vec3 geomViewDir;
in vec3 geomTexCoords;

void main()
{
	vec3 ecLightDirNormalized = normalize(geomLightDir);
	vec3 ecNormalNormalized = normalize(geomNormal);
	vec3 ecViewDirNormalized = normalize(geomViewDir);

	vec4 textureColor = texture(diffuseTexture, geomTexCoords.xy);

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
