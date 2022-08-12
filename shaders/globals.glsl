// ----------------------- GLOBALS ------------------------ //
#define PI        3.14159265358979323
#define TAU       6.28318530717958648
#define PHI       1.61803398874989484820459
#define INF       1000000.0
#define EPS       0.0001

struct Camera {
	vec3 origin, llc, horizontal, vertical, u, v, w;
	float lens_radius;
};

struct Ray {
	vec3 origin, direction;
};


//internal RNG state 
uvec4 seed;
ivec2 pixel;
// ------------------- END GLOBALS ------------------------ //
