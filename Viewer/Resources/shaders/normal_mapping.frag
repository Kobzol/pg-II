#version 400 core //compatibility

#include "structures.header"

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D texDiffuse;
uniform sampler2D texNormal;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in VS_OUT
{
	vec3 ecLightDir;
	vec3 ecNormal;
	vec3 ecViewDir;
	vec2 texCoords;
} vs_out;

vec3 remapNormal(vec3 texColor)
{
	return vec3(texColor.x * 2.0f - 1.0f, texColor.y * 2.0f - 1.0f, texColor.z);
}

void main()
{
	vec4 diffValue = texture(texDiffuse, vs_out.texCoords);
	vec3 normalValue = remapNormal(texture(texNormal, vs_out.texCoords).xyz);

	vec3 ecLightDirNormalized = normalize(vs_out.ecLightDir);
	vec3 ecNormalNormalized = normalize(vs_out.ecNormal + normalValue);
	vec3 ecViewDirNormalized = normalize(vs_out.ecViewDir);

	vec4 ambient = material.ambient * light.ambient;
	vec4 diffuse = (diffValue * material.diffuse) * light.diffuse;
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
