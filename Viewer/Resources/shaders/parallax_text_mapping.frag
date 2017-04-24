#version 400 core //compatibility

#include "structures.header"

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D diffuseTexture;
uniform float parallaxScale;
uniform float parallaxBias;
uniform float parallaxInvertDepth;
uniform float parallaxEffectHeight;

float parallaxStepCount = 32.0f;

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
vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height = texture(diffuseTexture, texCoords).r;
    vec2 p = (viewDir.xy / viewDir.z) * (height * parallaxScale + parallaxBias);
    return texCoords + p;
}
vec2 steepParallax(vec2 texCoords, vec3 viewDir)
{
	float height = texture(diffuseTexture, texCoords).r;
	vec2 p = (-viewDir.xy / viewDir.z) * (parallaxScale + parallaxBias);

	vec2 texDelta = p / parallaxStepCount;
	float stepValue = 1.0f / parallaxStepCount;
	float effectHeight = parallaxEffectHeight;
	while (height < effectHeight)
	{
		effectHeight -= stepValue;
		texCoords += texDelta;
		height = texture(diffuseTexture, texCoords).r;
	}

	vec2 delta = 0.5f * texDelta;
	vec2 mid = texCoords - delta;

	for (int i = 0; i < 5; i++)
	{
		delta *= 0.5f;

		if (texture(diffuseTexture, mid).r < effectHeight)
		{
			mid += delta;
		}
		else mid -= delta;
	}

	return mid;
}

void main()
{
	vec3 ecViewDirNormalized = normalize(vs_out.ecViewDir);
	vec2 texCoords = steepParallax(vs_out.texCoords, ecViewDirNormalized);

	if (texCoords.x > 1.0f || texCoords.y > 1.0f || texCoords.x < 0.0f || texCoords.y < 0.0f)
	{
		discard;
	}

	vec4 diffValue = texture(diffuseTexture, texCoords);
	vec3 ecLightDirNormalized = normalize(vs_out.ecLightDir);
	vec3 ecNormalNormalized = normalize(vs_out.ecNormal);

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
