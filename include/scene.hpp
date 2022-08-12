#pragma once

#include "options.hpp"
#include <camera.hpp>
#include <data.hpp>
#include <vector>

class Scene {

public:
  Camera *camera;

  std::vector<Triangle> triangles;
  std::vector<Normal> normals;
  std::vector<Vertex> vertices;
  std::vector<Material> materials;
  std::vector<Light> lights;

  Options options;

  void setCamera(Camera *camera);
};