// // ----------------------- HELPERS ------------------------ //
// void InitRNG(vec2 p, int frame) {
//     pixel = ivec2(p);
//     seed = uvec4(p, uint(frame), uint(p.x) + uint(p.y));
// }

// void pcg4d(inout uvec4 v) {
//     v = v * 1664525u + 1013904223u;
//     v.x += v.y * v.w; v.y += v.z * v.x; v.z += v.x * v.y; v.w += v.y * v.z;
//     v = v ^ (v >> 16u);
//     v.x += v.y * v.w; v.y += v.z * v.x; v.z += v.x * v.y; v.w += v.y * v.z;
// }

// float rand() {
//     pcg4d(seed); return float(seed.x) / float(0xffffffffu);
// }

vec3 toLinear(vec3 c)
{
    return pow(c,vec3(2.2));
}

vec3 toGamma(vec3 c)
{
    return pow(c,vec3(1./2.2));
}



//
// Hash functions by Nimitz:
// https://www.shadertoy.com/view/Xt3cDn
//



uint baseHash(uvec2 p)
{
    p=1103515245U*((p>>1U)^(p.yx));
    uint h32=1103515245U*((p.x)^(p.y>>3U));
    return h32^(h32>>16);
}

float g_seed=0.;


float hash1(inout float seed){
    uint n=baseHash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    return float(n)/float(0xffffffffU);
}

vec2 hash2(inout float seed){
    uint n=baseHash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    uvec2 rz=uvec2(n,n*48271U);
    return vec2(rz.xy&uvec2(0x7fffffffU))/float(0x7fffffff);
}

vec3 hash3(inout float seed)
{
    uint n=baseHash(floatBitsToUint(vec2(seed+=.1,seed+=.1)));
    uvec3 rz=uvec3(n,n*16807U,n*48271U);
    return vec3(rz&uvec3(0x7fffffffU))/float(0x7fffffff);
}



vec2 randomInUnitDisk(inout float seed){
    vec2 h=hash2(seed)*vec2(1.,TAU);
    float phi=h.y;
    float r=sqrt(h.x);
    return r*vec2(sin(phi),cos(phi));
}

vec3 randomInUnitSphere(inout float seed)
{
    vec3 h=hash3(seed)*vec3(2.,TAU,1.)-vec3(1.,0.,0.);
    float phi=h.y;
    float r=pow(h.z,1./3.);
    return r*vec3(sqrt(1.-h.x*h.x)*vec2(sin(phi),cos(phi)),h.x);
}

vec3 random_cos_weighted_hemisphere_direction( const vec3 n, inout float seed ) {
  	vec2 r = hash2(seed);
	vec3  uu = normalize(cross(n, abs(n.y) > .5 ? vec3(1.,0.,0.) : vec3(0.,1.,0.)));
	vec3  vv = cross(uu, n);
	float ra = sqrt(r.y);
	float rx = ra*cos(TAU*r.x); 
	float ry = ra*sin(TAU*r.x);
	float rz = sqrt(1.-r.y);
	vec3  rr = vec3(rx*uu + ry*vv + rz*n);
    return normalize(rr);
}


bool nearZero(vec3 v){
    if(abs(v.x)<1e-8&&abs(v.y)<1e-8&&abs(v.z)<1e-8){
        return true;
    }
    return false;
}

float atan2(float a,float b)
{
    return asin(a)>0.?acos(b):-acos(b);
}

void initRNG(vec2 coord, float time) {
    g_seed=float(baseHash(floatBitsToUint(coord)))/float(0xffffffffU) + time;
}



vec3 bary(vec3 p, vec3 v0, vec3 v1, vec3 v2) {
    // returns barycentric coordinates of p in triangle (v0,v1,v2)
    vec3 v0v1 = v1 - v0;
    vec3 v0v2 = v2 - v0;
    
    vec3 pv0 = p - v0;
    vec3 pv1 = p - v1;
    vec3 pv2 = p - v2;

    float d00 = dot(v0v1, v0v1);
    float d01 = dot(v0v1, v0v2);
    float d11 = dot(v0v2, v0v2);
    float d20 = dot(pv0, v0v1);
    float d21 = dot(pv0, v0v2);

    float d00d11 = d00 * d11 - d01 * d01;

    float v = (d11 * d20 - d01 * d21) / d00d11;
    float w = (d00 * d21 - d01 * d20) / d00d11;
    float u = 1.0 - v - w;

    return vec3(u,v,w);
}


// ----------------------- END HELPERS --------------------- //
