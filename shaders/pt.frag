#version 410

out vec4 color;
in vec2 TexCoords;

#include globals.glsl
#include uniforms.glsl
#include helpers.glsl
#include hit.glsl
#include scatter.glsl


precision highp float;

vec3 sky(Ray r){
    float t=.5*(normalize(r.direction).y+1.);
    return(1.-t)*vec3(1.)+t*vec3(.5,.7,1.);
}

Ray get_ray(Camera cam, vec2 uv) {
    vec3 rd = cam.lens_radius * randomInUnitSphere(g_seed);
    vec3 offset = cam.u * rd.x + cam.v * rd.y;

    return Ray(cam.origin + offset, normalize(cam.llc+uv.x*cam.horizontal+uv.y*cam.vertical-cam.origin-offset));
}

Material lambert() {
    Material m;
    m.color = vec3(0.8, 0.8, 0.8);

    return m;
}

bool worldIntersect(Ray r,float tmin,float tmax, out HitRecord hit){

    HitRecord temp;
    bool hit_anything = false;
    hit.dist = tmax;

    for(int i=0;i<12;i++){
        if(triHit(i,r,tmin,hit.dist,temp)){
            hit_anything = true;
            hit = temp;
        }
    }

    Sphere s1 = Sphere(vec3(1.0, 0, 2.0), 0.5, Material(vec3(0.8, 0.3, 0.3)));
    if (hit_sphere(s1, r, tmin, hit.dist, hit)) {
        hit_anything = true;
    }

    // Sphere s2 = Sphere(vec3(1.0, 0, 0), 0.5, Material(vec3(0.8, 0.3, 0.8)));
    // if (hit_sphere(s2, r, tmin,hit.dist, hit)) {
    //     hit_anything = true;
    // }

    Sphere s3 = Sphere(vec3(0, -1000.5, 0), 1000, Material(vec3(0.8, 0.8, 0.8)));
    if (hit_sphere(s3, r, tmin, hit.dist, hit)) {
        hit_anything = true;
    }
 


    return hit_anything;
}



vec3 pathtrace(Ray r){
  HitRecord hit;
  vec3 c=vec3(1.);
  vec3 background = sky(r); 
  for(int i=0;i<depthMax;++i){
    // if(worldIntersect(r,EPS,INF,hit)){
    if(closestHit(r, hit)){
      Ray scattered;
      vec3 attenuation;
      vec3 emitted;
      if(scatter(r,hit,attenuation,emitted,scattered)){
        c*=emitted+attenuation;
        r=scattered;
      }else{
        c*=emitted;
        return c;
      }
    }else{
      c*=background;
      return c;
    }
  }
//   return c;

  return c;
}



vec3 trace(Ray r){
  return pathtrace(r) / frameNum;
}



void main() {
    initRNG(gl_FragCoord.xy, time);

    vec4 accumColor = texture(accTex, TexCoords);
    vec3 accLin = accumColor.xyz * frameNum;

    vec2 r_uv = (gl_FragCoord.xy + hash2(g_seed))/resolution;
    Ray r = get_ray(camera, r_uv);


    vec3 col = pathtrace(r);

    color = vec4((col + accLin)/(frameNum + 1), 1.0);
}