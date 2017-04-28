#version 400 core //compatibility

#include "structures.header"

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec3 texCoords;
in vec4 fragPosLightSpace;

float ShadowCalculation(vec4 position, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = position.xyz / position.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow
    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);  
	float shadow = 0.0f;
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}    
	}
	shadow /= 9.0f;

	if(projCoords.z > 1.0f)
	{
		shadow = 0.0f;
	}

    return shadow;
}

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

	float shadow = ShadowCalculation(fragPosLightSpace, ecNormalNormalized, ecLightDirNormalized);
	FragColor = ambient + (1.0f - shadow) * (diffuse + specular);
}
