#version 420 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D diffuseTexture;

#define MAX_POINTS 30
uniform vec2 Points[MAX_POINTS];
uniform float Time;
uniform float VoronoiScale;

in vec2 texCoords;
in vec2 centerVector;

vec2 random2(vec2 p)
{
	return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

vec4 getDistColor()
{
	vec2 pos = centerVector * VoronoiScale;
	vec2 i_st = floor(pos);
    vec2 f_st = fract(pos);
	float dist = 1.0f;

	vec2 minPoint = vec2(0.0f, 0.0f);

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			vec2 neighbor = vec2(float(x), float(y));
			vec2 point = random2(i_st + neighbor);
			point = 0.5 + 0.5 * sin(Time + 6.2831 * point);

			vec2 diff = neighbor + point - f_st;
			float currDist = length(diff);

			if (currDist < dist)
			{
				dist = currDist;
				minPoint = neighbor + point;
			}
		}
	}

    if (dist < 0.02f)
	{
		return vec4(1.0f);
	}
	else return vec4(minPoint.x, minPoint.y, 0.0f, 1.0f) - abs(sin(80.0f * dist)) * 0.07;
}
vec4 getDistPointsColor()
{
	float dist = 1.0f;
	vec2 minPoint = vec2(0.0f, 0.0f);

	for (int i = 0; i < MAX_POINTS; i++)
	{
		vec2 point = Points[i];
		point = 0.5 + 0.5 * sin(Time + 6.2831 * point);

		float currDist = distance(centerVector, point);
		if (currDist < dist)
		{
			dist = currDist;
			minPoint = point;
		}
	}

	return vec4(minPoint.x, minPoint.y, 0.0f, 1.0f);
}

void main()
{
	vec4 color = getDistColor();

	FragColor = color;
}
