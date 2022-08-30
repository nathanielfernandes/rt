#pragma once

#include <glm/glm.hpp>

//    vertex/normal
//       ___|___
// vec4 (x, y, z, w)
//                |
//           uv texture coords

typedef glm::vec4 VertexU;
typedef glm::vec4 NormalV;

// x,y,z are indices pointing to vertices/normals in the vertex/normal array
// w is the materialID
typedef glm::ivec4 Triangle;

#define LAMBERTIAN 0.0
#define METAL 1.0
#define DIELECTRIC 2.0
#define EMISSIVE 3.0

struct Material {
  float materialType; // 0 = lambertian, 1 = metal, 2 = dielectric, 3 = emissive
  float ir;           // index of refraction
  float fuzz;         // fuzziness float any;
  float any;
  // glm::vec4 params;
  glm::vec4 albedo;

  Material() {
    materialType = LAMBERTIAN;
    ir = 0.0f;
    fuzz = 0.0f;
    any = 0.0f;
    // params = glm::vec4(0.0f);
    albedo = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
  }
};

struct Light {
  glm::vec3 position;
  glm::vec3 emission;

  glm::vec3 u;
  glm::vec3 v;
};