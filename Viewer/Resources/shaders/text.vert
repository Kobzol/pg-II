#version 400 core //compatibility

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 TextProjection;

void main()
{
    gl_Position = TextProjection * vec4(vertex.xy, 0.0f, 1.0f);
    TexCoords = vertex.zw;
}
