#version 400 core //compatibility

in vec2 TexCoords;

out vec4 color;

uniform sampler2D TextTexture;
uniform vec3 TextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(TextTexture, TexCoords).r);
    color = vec4(TextColor, 1.0) * sampled;
}
