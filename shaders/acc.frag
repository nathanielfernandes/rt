#version 410

out vec4 color;
in vec2 TexCoords;

uniform sampler2D pathTraceTexture;

void main()
{
	color = texture(pathTraceTexture, TexCoords);
}