vec3 ray_at(Ray r,float t){
    return r.origin+r.direction*t;
}

bool hit_sphere(Sphere s,Ray r,float t_min,float t_max,inout HitRecord hit){
  vec3 oc=r.origin-s.center;
    float b=dot(oc,r.direction);
    float c=dot(oc,oc)-s.radius*s.radius;
    float d=b*b-c;
    
    if(d<0.){
        return false;
    }
    float t1=(-b-sqrt(d));
    float t2=(-b+sqrt(d));
    float t=t1<t_min?t2:t1;
    
    vec3 p=ray_at(r,t);
    vec3 n=(p-s.center);
    // if front_face, the ray is in the sphere and not out, so invert normal
    bool front_face=dot(r.direction,n)>0.;
    
    if(t<t_min||t>t_max){
        return false;
    }
    n=front_face?-n:n;
    n/=s.radius;
    
    vec3 outward_normal=(hit.point-s.center)/s.radius;

    if(t<t_max){
        hit.dist=t;
        hit.point=p;
        hit.normal=n;
        hit.ffnormal = front_face?outward_normal:-outward_normal;
        hit.mat=s.mat;
        // hit.uv=uv;
    }
    
    return true;
}

bool triHit(int index, Ray r, float t_min, float t_max, inout HitRecord hit) {
    ivec3 indices = texelFetch(trianglesTex, index).xyz;


    vec4 v0 = texelFetch(verticesTex, indices.x);
    vec4 v1 = texelFetch(verticesTex, indices.y);
    vec4 v2 = texelFetch(verticesTex, indices.z);

    vec3 v0v1 = v1.xyz - v0.xyz;
    vec3 v0v2 = v2.xyz - v0.xyz;

    vec3 pvec = cross(r.direction, v0v2);
    float det = dot(v0v1, pvec);
    if (det < EPS) {
        return false;
    }
    float invDet = 1.0 / det;
    vec3 tvec = r.origin - v0.xyz;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    vec3 qvec = cross(tvec, v0v1);
    float v = dot(r.direction, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    float t = dot(v0v2, qvec) * invDet;
    if (t < t_min || t > t_max) {
        return false;
    }

    hit.dist = t;
    hit.point = r.origin + r.direction * t;
    hit.bary = vec3(1.0 - u - v, u, v); // bary(hit.point, v0.xyz, v1.xyz, v2.xyz);
    
    vec4 n0 = texelFetch(normalsTex, indices.x);
    vec4 n1 = texelFetch(normalsTex, indices.y);
    vec4 n2 = texelFetch(normalsTex, indices.z);

    vec2 t0 = vec2(v0.w, n0.w);
    vec2 t1 = vec2(v1.w, n1.w);
    vec2 t2 = vec2(v2.w, n2.w);

    hit.uv = t0 * hit.bary.x + t1 * hit.bary.y + t2 * hit.bary.z;
    hit.normal = normalize(n0.xyz * hit.bary.x + n1.xyz * hit.bary.y + n2.xyz * hit.bary.z);

    // hit.normal = normalize(cross(v0v1, v0v2));
    hit.ffnormal = dot(hit.normal, r.direction) <= 0.0 ? hit.normal : -hit.normal;

    // // hit.mat = Material(vec3(0.8, 0.3, 0.3));
    hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));

    return true;

    // vec3 pvec = cross(r.direction, v0v2);
    // float det = dot(v0v1, pvec);

    // vec3 tvec = r.origin - v0.xyz;
    // vec3 qvec = cross(tvec, v0v1);

    // vec4 uvt;
    // uvt.x = dot(tvec, pvec);
    // uvt.y = dot(r.direction, qvec);
    // uvt.z = dot(v0v2, qvec);
    // uvt.xyz = uvt.xyz / det;
    // uvt.w = 1.0 - uvt.x - uvt.y;


    // if (all(greaterThanEqual(uvt, vec4(0.0))) && uvt.z < t_max) {
    //     hit.dist = uvt.z;
    //     hit.point = ray_at(r, uvt.z) + r.direction * EPS;
    //     // hit.bary = uvt.wxy;
    //     hit.bary = bary(hit.point, v0.xyz, v1.xyz, v2.xyz);

    //     // Normals
    //     vec4 n0 = texelFetch(normalsTex, indices.x);
    //     vec4 n1 = texelFetch(normalsTex, indices.y);
    //     vec4 n2 = texelFetch(normalsTex, indices.z);

    //     vec2 t0 = vec2(v0.w, n0.w);
    //     vec2 t1 = vec2(v1.w, n1.w);
    //     vec2 t2 = vec2(v2.w, n2.w);

    //     hit.uv = t0 * hit.bary.x + t1 * hit.bary.y + t2 * hit.bary.z;
    //     hit.normal = normalize(n0.xyz * hit.bary.x + n1.xyz * hit.bary.y + n2.xyz * hit.bary.z);
    //     // hit.normal = normalize(cross(v0v1, v0v2));
    //     hit.ffnormal = dot(hit.normal, r.direction) <= 0.0 ? hit.normal : -hit.normal;

    //     hit.mat = Material(vec3(0.8, 0.3, 0.3));
    //     // hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));
    //     return true;

    //     // hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));
    // }


    // return false;


    // Normals
    // vec4 n0 = texelFetch(normalsTex, indices.x);
    // vec4 n1 = texelFetch(normalsTex, indices.y);
    // vec4 n2 = texelFetch(normalsTex, indices.z);



    // hit.normal = texelFetch(normalsTex, index).xyz;
    // hit.normal = normalize(cross(v0v1, v0v2));
    // hit.mat = Material(vec3(0.8, 0.3, 0.3));
    // hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));

}


// bool triangleHit(Ray r, int index, inout HitRecord hit) {
//     ivec3 indices = texelFetch(trianglesTex, index).xyz;

//     vec4 v0 = texelFetch(verticesTex, indices.x);
//     vec4 v1 = texelFetch(verticesTex, indices.y);
//     vec4 v2 = texelFetch(verticesTex, indices.z);

//     vec3 v0v1 = v1.xyz - v0.xyz;
//     vec3 v0v2 = v2.xyz - v0.xyz;

//     vec3 pvec = cross(r.direction, v0v2);
//     float det = dot(v0v1, pvec);

//     vec3 tvec = r.origin - v0.xyz;
//     vec3 qvec = cross(tvec, v0v1);

//     vec4 uvt;
//     uvt.x = dot(tvec, pvec);
//     uvt.y = dot(r.direction, qvec);
//     uvt.z = dot(v0v2, qvec);
//     uvt.xyz = uvt.xyz / det;
//     uvt.w = 1.0 - uvt.x - uvt.y;


//     if (all(greaterThanEqual(uvt, vec4(0.0))) && uvt.z < hit.dist) {
//         hit.dist = uvt.z;
//         hit.point = ray_at(r, uvt.z) ;// + r.direction * EPS;
//         hit.bary = uvt.wxy;
//         // hit.bary = bary(hit.point, v0.xyz, v1.xyz, v2.xyz);

//         // Normals
//         vec4 n0 = texelFetch(normalsTex, indices.x);
//         vec4 n1 = texelFetch(normalsTex, indices.y);
//         vec4 n2 = texelFetch(normalsTex, indices.z);

//         // vec2 t0 = vec2(v0.w, n0.w);
//         // vec2 t1 = vec2(v1.w, n1.w);
//         // vec2 t2 = vec2(v2.w, n2.w);

//         // hit.uv = t0 * hit.bary.x + t1 * hit.bary.y + t2 * hit.bary.z;
//         hit.normal = normalize(n0.xyz * hit.bary.x + n1.xyz * hit.bary.y + n2.xyz * hit.bary.z);
//         // hit.normal = normalize(cross(v0v1, v0v2));
//         hit.ffnormal = dot(hit.normal, r.direction) <= 0.0 ? hit.normal : -hit.normal;

//         // hit.mat = Material(vec3(0.8, 0.3, 0.3));
//         hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));
//         return true;

//         // hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));
//     }

//     return false;
// }

bool triangleHit(Ray r, int index, inout HitRecord hit) {
    ivec3 indices = texelFetch(trianglesTex, index).xyz;

    vec4 v0 = texelFetch(verticesTex, indices.x);
    vec4 v1 = texelFetch(verticesTex, indices.y);
    vec4 v2 = texelFetch(verticesTex, indices.z);

    vec3 v0v1 = v1.xyz - v0.xyz;
    vec3 v0v2 = v2.xyz - v0.xyz;

    vec3 pvec = cross(r.direction, v0v2);
    float det = dot(v0v1, pvec);
    if (det < EPS) {
        return false;
    }
    float invDet = 1.0 / det;
    vec3 tvec = r.origin - v0.xyz;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    vec3 qvec = cross(tvec, v0v1);
    float v = dot(r.direction, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    float t = dot(v0v2, qvec) * invDet;
    
    if (t < EPS || t > hit.dist) {
        return false;
    }

    hit.v0 = v0;
    hit.v1 = v1;
    hit.v2 = v2;
    hit.ID = indices;

    hit.dist = t;
    hit.bary = vec3(1.0 - u - v, u, v); // bary(hit.point, v0.xyz, v1.xyz, v2.xyz);
    
    // vec4 n0 = texelFetch(normalsTex, indices.x);
    // vec4 n1 = texelFetch(normalsTex, indices.y);
    // vec4 n2 = texelFetch(normalsTex, indices.z);

    // vec2 t0 = vec2(v0.w, n0.w);
    // vec2 t1 = vec2(v1.w, n1.w);
    // vec2 t2 = vec2(v2.w, n2.w);

    // hit.uv = t0 * hit.bary.x + t1 * hit.bary.y + t2 * hit.bary.z;
    // hit.normal = normalize(n0.xyz * hit.bary.x + n1.xyz * hit.bary.y + n2.xyz * hit.bary.z);

    // // hit.normal = normalize(cross(v0v1, v0v2));
    // hit.ffnormal = dot(hit.normal, r.direction) <= 0.0 ? hit.normal : -hit.normal;

    // // // hit.mat = Material(vec3(0.8, 0.3, 0.3));
    // hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));

    return true;
}


void addNormalUV(inout HitRecord hit, Ray r) {
    vec4 n0 = texelFetch(normalsTex, hit.ID.x);
    vec4 n1 = texelFetch(normalsTex, hit.ID.y);
    vec4 n2 = texelFetch(normalsTex,  hit.ID.z);

    vec2 t0 = vec2(hit.v0.w, n0.w);
    vec2 t1 = vec2(hit.v1.w, n1.w);
    vec2 t2 = vec2(hit.v2.w, n2.w);

    hit.point = r.origin + r.direction * hit.dist;

    hit.uv = t0 * hit.bary.x + t1 * hit.bary.y + t2 * hit.bary.z;
    hit.normal = normalize(n0.xyz * hit.bary.x + n1.xyz * hit.bary.y + n2.xyz * hit.bary.z);

    hit.ffnormal = dot(hit.normal, r.direction) <= 0.0 ? hit.normal : -hit.normal;

    // hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));
    hit.mat = Material(vec3(0.8, 0.8, 0.8));
}

float aabbHit(Ray r, vec3 mmin, vec3 mmax) {
    vec3 invdir = 1.0 / r.direction;

	vec3 f = (mmax - r.origin) * invdir;
	vec3 n = (mmin - r.origin) * invdir;

	vec3 tmax = max(f, n);
	vec3 tmin = min(f, n);

	float t1 = min(tmax.x, min(tmax.y, tmax.z));
	float t0 = max(tmin.x, max(tmin.y, tmin.z));

	return (t1 >= t0) ? (t0 > 0.f ? t0 : t1) : -1.0;
}



bool closestHit(Ray r, inout HitRecord hit) {
    hit.dist = INF;

    int stack[64];
    int ptr = 0;
    stack[ptr++] = -1;

    int index = bvhTop;
    float left = 0.0;
    float right = 0.0;

    bool blas = false;

    while (index != -1) {
        ivec3 leafs = ivec3(texelFetch(BVHTex, index * 3 + 2));

        int leftNode = leafs.x;
        int rightNode = leafs.y;

        // is leaf
        if (leafs.z == 1) {
            for (int i=0; i<=rightNode; i++) {
                triangleHit(r, leftNode + i, hit);
            }
        // } 
        // else if (leafs.z < 0) {
        //     stack[ptr++] = -1;
        //     index = leftNode;
        //     blas = true;
        //     continue;
        } else {
            left = aabbHit(r, texelFetch(BVHTex, leftNode * 3 + 0).xyz, texelFetch(BVHTex, leftNode * 3 + 1).xyz);
            right = aabbHit(r, texelFetch(BVHTex, rightNode * 3 + 0).xyz, texelFetch(BVHTex, rightNode * 3 + 1).xyz);

            if (left > 0.0 && right > 0.0) {
                    int deferred = -1;
                    if (left > right) {
                        index = rightNode;
                        deferred = leftNode;
                    } else {
                        index = leftNode;
                        deferred = rightNode;
                    }

                    stack[ptr++] = deferred;
                    continue;
                } else if (left > 0.0) {
                    index = leftNode;
                    continue;
                } else if (right > 0.0) {
                    index = rightNode;
                    continue;
                }
            }
        index = stack[--ptr];

        // if (blas && index == -1) {
        //     blas = false;
        //     continue;
        // }
    }

    if (hit.dist == INF) {
        return false;
    }

    addNormalUV(hit, r);

    return true;
}






// float closestHit(Ray r, inout HitRecord hit) {
//     float t = INF;

//     int stack[64];
// 	int ptr = 0;
// 	stack[ptr++] = -1;

// 	int idx = 0;
// 	float leftHit = 0.0;
// 	float rightHit = 0.0;

// 	while (idx > -1)
// 	{
// 		int n = idx;
// 		vec3 LRLeaf = texelFetch(BVHTex, n * 3 + 2).xyz;

// 		int leftIndex = int(LRLeaf.x);
// 		int rightIndex = int(LRLeaf.y);
// 		int isLeaf = int(LRLeaf.z);

// 		if (isLeaf == 1)
// 		{
// 			for (int i = 0; i <= rightIndex; i++) // Loop through indices
// 			{
// 				int index = leftIndex + i;
// 				vec4 triIndex = texelFetch(trianglesTex, index).xyzw;

// 				vec3 v0 = texelFetch(verticesTex, int(triIndex.x)).xyz;
// 				vec3 v1 = texelFetch(verticesTex, int(triIndex.y)).xyz;
// 				vec3 v2 = texelFetch(verticesTex, int(triIndex.z)).xyz;

// 				vec3 e0 = v1 - v0;
// 				vec3 e1 = v2 - v0;
// 				vec3 pv = cross(r.direction, e1);
// 				float det = dot(e0, pv);

// 				vec3 tv = r.origin - v0.xyz;
// 				vec3 qv = cross(tv, e0);

// 				vec4 uvt;
// 				uvt.x = dot(tv, pv);
// 				uvt.y = dot(r.direction, qv);
// 				uvt.z = dot(e1, qv);
// 				uvt.xyz = uvt.xyz / det;
// 				uvt.w = 1.0 - uvt.x - uvt.y;

// 				if (all(greaterThanEqual(uvt, vec4(0.0))) && uvt.z < t)
// 				{
// 					t = uvt.z;

//                     hit.point = ray_at(r, uvt.z) ;// + r.direction * EPS;
//                     hit.bary = uvt.wxy;
//                     // hit.bary = bary(hit.point, v0.xyz, v1.xyz, v2.xyz);
//                     // Normals
//                     vec4 n0 = texelFetch(normalsTex, int(triIndex.x));
//                     vec4 n1 = texelFetch(normalsTex, int(triIndex.y));
//                     vec4 n2 = texelFetch(normalsTex, int(triIndex.z));

//                     // vec2 t0 = vec2(v0.w, n0.w);
//                     // vec2 t1 = vec2(v1.w, n1.w);
//                     // vec2 t2 = vec2(v2.w, n2.w);

//                     // hit.uv = t0 * hit.bary.x + t1 * hit.bary.y + t2 * hit.bary.z;
//                     hit.normal = normalize(n0.xyz * hit.bary.x + n1.xyz * hit.bary.y + n2.xyz * hit.bary.z);
//                     // hit.normal = normalize(cross(v0v1, v0v2));
//                     hit.ffnormal = dot(hit.normal, r.direction) <= 0.0 ? hit.normal : -hit.normal;

//                     // hit.mat = Material(vec3(0.8, 0.3, 0.3));
//                     hit.mat = Material(0.5* vec3(hit.normal.x + 1.0, hit.normal.y + 1.0, hit.normal.z + 1.0));
// 				}
// 			}
// 		}
// 		else
// 		{
// 			leftHit = aabbHit(r, texelFetch(BVHTex, leftIndex * 3 + 0).xyz, texelFetch(BVHTex, leftIndex * 3 + 1).xyz);
// 			rightHit = aabbHit(r, texelFetch(BVHTex, rightIndex * 3 + 0).xyz, texelFetch(BVHTex, rightIndex * 3 + 1).xyz);

// 			if (leftHit > 0.0 && rightHit > 0.0)
// 			{
// 				int deferred = -1;
// 				if (leftHit > rightHit)
// 				{
// 					idx = rightIndex;
// 					deferred = leftIndex;
// 				}
// 				else
// 				{
// 					idx = leftIndex;
// 					deferred = rightIndex;
// 				}

// 				stack[ptr++] = deferred;
// 				continue;
// 			}
// 			else if (leftHit > 0)
// 			{
// 				idx = leftIndex;
// 				continue;
// 			}
// 			else if (rightHit > 0)
// 			{
// 				idx = rightIndex;
// 				continue;
// 			}
// 		}
// 		idx = stack[--ptr];
// 	}

// 	hit.dist = t;
// 	return t;

// }


