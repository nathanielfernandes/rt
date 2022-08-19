#pragma once

#include "mesh.hpp"
#include <RadeonRays/bvh_translator.hpp>
#include <camera.hpp>
#include <data.hpp>
#include <glu.hpp>

#include <options.hpp>
#include <vector>

class Scene {

public:
  Scene() : camera(nullptr), initialized(false) {
    sceneBvh = new RadeonRays::Bvh(10.0f, 64, false);
  }
  Camera *camera;

  std::vector<Mesh *> meshes;

  std::vector<Triangle> triangles;
  std::vector<VertexU> vertices;
  std::vector<NormalV> normals;

  std::vector<MeshInstance> meshInstances;

  RadeonRays::BvhTranslator bvhTranslator;
  RadeonRays::bbox sceneBounds;

  // std::vector<Material> materials;
  // std::vector<Light> lights;

  Options options;
  bool initialized{false};

  int AddMesh(const std::string &fp);
  int AddMeshInstance(const MeshInstance &meshInstance);

  void ProcessScene();
  void RebuildInstances();

  void setCamera(Camera *camera);

private:
  RadeonRays::Bvh *sceneBvh;
  void createBLAS();
  void createTLAS();
};