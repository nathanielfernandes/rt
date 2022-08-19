// --------------------- UNIFORMS ------------------------- //
uniform vec2 resolution;

uniform sampler2D accTex;

uniform samplerBuffer BVHTex;
uniform isamplerBuffer trianglesTex;
uniform samplerBuffer verticesTex;
uniform samplerBuffer normalsTex;

// temp
uniform int numTris;

uniform int totalLights;
uniform int bvhTop;
uniform int frameNum;
uniform int depthMax;
uniform float time;
// -------------------- END UNIFORMS ---------------------- //