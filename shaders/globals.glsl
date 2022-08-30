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

uniform Camera camera;

struct Ray {
	vec3 origin, direction;
};

// temp Materials
#define LAMBERTIAN 0
#define METAL 1
#define DIELECTRIC 2
#define EMISSIVE 3

struct Material {
	int type;
	float ir;
	float fuzz;
	
   	vec3 albedo;
};

struct HitRecord {
	//              front facing normal
	vec3 point, normal, ffnormal, bary;
	vec2 uv;
	float dist;
	// int ID, matID;
	ivec3 ID;
	int matID;

	vec4 v0, v1, v2;
	Material mat;
};


struct Sphere{
    vec3 center;
    float radius;
    Material mat;
};

// ------------------- END GLOBALS ------------------------ //
