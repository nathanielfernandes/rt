#version 410

out vec3 color;
in vec2 TexCoords;

#include uniforms.glsl
#include globals.glsl
#include helpers.glsl

void main(void) {
	InitRNG(gl_FragCoord.xy, 1);

	float r1 = 2.0 * rand();
	float r2 = 2.0 * rand();

	vec3 accumColor = texture(accTex, TexCoords).xyz;

	vec3 pixelColor = vec3(0.0001, 0.0, 0.0001);

	color = pixelColor + accumColor;
}