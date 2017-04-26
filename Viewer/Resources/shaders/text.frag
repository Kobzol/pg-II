#version 400 core //compatibility

in vec2 TexCoords;

out vec4 color;

uniform sampler2D TextTexture;
uniform vec3 TextColor;
uniform float TextAlpha;

void main()
{
	color = vec4(TextColor * texture(TextTexture, TexCoords).r, TextAlpha);
}
