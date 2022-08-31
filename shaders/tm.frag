#version 330

out vec4 color;
in vec2 TexCoords;

uniform sampler2D ptTex;
uniform float invSampleNum;


vec4 ToneMap(in vec4 c, float limit)
{
	float luminance = 0.3*c.x + 0.6*c.y + 0.1*c.z;
	return c * 1.0/(1.0 + luminance/limit);
}

void main() {
	// color = texture(ptTex, TexCoords) * invSampleNum;
	// color = pow(ToneMap(color,20.0), vec4(1.0 / 2.2));
	// color = vec4(invSampleNum, 0.0, 0.0, 1.0);
    color = texture(ptTex, TexCoords);
}