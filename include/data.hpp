#pragma once

#include <glm/glm.hpp>

struct Triangle {
  glm::vec4 indices;
};

struct Normal {
  glm::vec3 normals[3];
};

// struct TexCoord {
//   glm::vec2 texcoords[3];
// };

struct Vertex {
  glm::vec3 vertex;
};

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