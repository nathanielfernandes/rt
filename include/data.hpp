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
typedef glm::ivec3 Triangle;

struct Material {
  Material() {
    albedo = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    emission = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    params = glm::vec4(0.0f, 0.5f, 0.0f, 0.0f);
    // texIDs = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f);
  };
  glm::vec4 albedo; // layout: R,G,B, MaterialType
  glm::vec4 emission;
  glm::vec4 params; // layout: metallic, roughness, IOR, transmittance
  //   glm::vec4 texIDs; // layout: (Texture Map IDs) albedo ID,
  //   metallicRoughness
  //                     // ID, normalMap ID
};

struct Light {
  glm::vec3 position;
  glm::vec3 emission;

  glm::vec3 u;
  glm::vec3 v;
};