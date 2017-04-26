#version 420 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D Frame;
uniform sampler2D FrameAccum;
uniform float AccumDecayRatio;

in vec2 texCoords;
in vec2 centerVector;

void main()
{
	//FragColor = texture(Frame, texCoords) + texture(FrameAccum, texCoords) * AccumDecayRatio;
	FragColor = mix(texture(Frame, texCoords), texture(FrameAccum, texCoords), AccumDecayRatio);
}
