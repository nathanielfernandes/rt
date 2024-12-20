
bool lambertian_scatter(Ray r,HitRecord hit,out vec3 attenuation,out Ray scattered){
    vec3 target = hit.point+hit.normal+randomInUnitSphere(g_seed);
    scattered=Ray(hit.point,normalize(target-hit.point));
    // scattered=Ray(hit.point,random_cos_weighted_hemisphere_direction(hit.normal,g_seed));
    attenuation=hit.mat.albedo;
    return true;
}

bool metal_scatter(Ray r,HitRecord hit,out vec3 attenuation,out Ray scattered){
    vec3 reflected=reflect(normalize(r.direction),hit.normal);
    if(dot(reflected,hit.normal)>0.){
        scattered=Ray(hit.point,reflected+hit.mat.fuzz*randomInUnitSphere(g_seed));
        // attenuation=texure_color_at(hit);
        attenuation=hit.mat.albedo;
        return true;
    }
    return false;
}

float schlick(float cosine,float ref_idx){
    float r0=(1.-ref_idx)/(1.+ref_idx);
    r0*=r0;
    
    return r0+(1.-r0)*pow((1.-cosine),5.);
}

vec3 c_refract(vec3 uv,vec3 normal,float etai_over_etai){
    float cos_theta=min(dot(-normalize(uv),normal),1.);
    vec3 r_out_perp=etai_over_etai*(uv+cos_theta*normal);
    vec3 r_out_parallel=-sqrt(abs(1.-pow(length(r_out_perp),2.)))*normal;
    return r_out_perp+r_out_parallel;
    
}

bool dielectric_scatter(Ray r,HitRecord hit,out vec3 attenuation,out Ray scattered){
    
    attenuation=vec3(1.);
    float eta=hit.mat.ir;
    vec3 normal=hit.normal;
    if(dot(hit.normal, hit.ffnormal) > 0.0){
        eta=1./eta;
    }
    
    float cos_theta=min(dot(-normalize(r.direction),normal),1.);
    float sin_theta=sqrt(1.-cos_theta*cos_theta);
    
    bool cannot_refract=sin_theta*eta>1.;
    
    if(cannot_refract||schlick(cos_theta,eta)>hash1(g_seed)){
        scattered=Ray(hit.point,reflect(r.direction,normal));
    }else{
        scattered=Ray(hit.point,c_refract(r.direction,normal,eta));
    }
    
    return true;
    
}

bool scatter(Ray r,HitRecord hit,out vec3 attenuation,out vec3 emitted,out Ray scattered){
    emitted=vec3(0.);
    // return lambertian_scatter(r,hit,attenuation,scattered);

    if(hit.mat.type==LAMBERTIAN){
        return lambertian_scatter(r,hit,attenuation,scattered);
    }
    
    if(hit.mat.type==METAL){
        return metal_scatter(r,hit,attenuation,scattered);
    }
    
    if(hit.mat.type==DIELECTRIC){
        return dielectric_scatter(r,hit,attenuation,scattered);
    }
    
    if(hit.mat.type==EMISSIVE){
        // emitted=texure_color_at(hit);
        emitted=hit.mat.albedo;
        return false;
    }
    
    return false;
}