#version 420 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D Frame;
uniform float InterferenceRatio;
uniform int InterferenceEffect;

in vec2 texCoords;
in vec2 centerVector;

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898f, 78.233f))) * 43758.5453123f);
}

const float minOffsetInvert = 0.1f;

void main()
{
	vec2 coords = texCoords;
	vec4 color;

	if (InterferenceRatio > 0.0f)
	{
		vec2 offset = vec2(0.0f, 0.0f);

		if (InterferenceEffect == 0)
		{
			vec4 offsetColor;
			offset.x = random(coords) * 0.01f;
			offset.x += sin(coords.y * 100.0f) * 0.01f;
			coords += offset * InterferenceRatio;

			vec2 coords2 = texCoords;
			coords2.x -= 0.005f * InterferenceRatio;

			offsetColor = texture(Frame, coords2);
			if (offsetColor.r > minOffsetInvert || offsetColor.g > minOffsetInvert || offsetColor.b > minOffsetInvert)
			{
				offsetColor += InterferenceRatio;
			}

			color = mix(texture(Frame, coords), offsetColor, 0.5f);
		}
		else if (InterferenceEffect == 1)
		{
			offset.x = random(coords * InterferenceRatio) * 0.01f;
			offset.x = sin(coords.y * 50.0f * InterferenceRatio) * 0.01f;
			color = texture(Frame, coords + (offset * InterferenceRatio));
		}
		else if (InterferenceEffect == 2)
		{
			offset.x = random(coords * InterferenceRatio) * 0.02f;
			color = texture(Frame, coords + (offset * InterferenceRatio));
		}
	}
	else color = texture(Frame, coords);
	
	FragColor = color;
}
