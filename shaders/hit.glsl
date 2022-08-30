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




bool triangleHit(Ray r, int index, inout HitRecord hit) {
    ivec4 indices = texelFetch(trianglesTex, index);

    vec4 v0 = texelFetch(verticesTex, int(indices.x));
    vec4 v1 = texelFetch(verticesTex, int(indices.y));
    vec4 v2 = texelFetch(verticesTex, int(indices.z));

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
    hit.ID = indices.xyz;
    hit.matID = indices.w;

    hit.dist = t;
    hit.bary = vec3(1.0 - u - v, u, v); // bary(hit.point, v0.xyz, v1.xyz, v2.xyz);

    return true;
}


void addMaterial(inout HitRecord hit) {
    Material mat;

    int index = hit.matID * 2;

    vec4 param1 = texelFetch(materialsTex, index + 0);
    vec4 param2 = texelFetch(materialsTex, index + 1);

    mat.type = int(param1.x);
    mat.albedo = param2.rgb;

    // mat.albedo = vec3(0.2, 0.2, 0.2);
    hit.mat = mat;
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
    // hit.mat = Material(vec3(0.8, 0.8, 0.8));
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
    addMaterial(hit);

    return true;
}





