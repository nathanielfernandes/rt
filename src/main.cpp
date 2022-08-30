#include "load.hpp"
#include "mesh.hpp"
#include "options.hpp"
#include <app.hpp>
#include <iostream>

auto main() -> int {
  Scene *scene = new Scene();
  Options options = Options();

  options.samples = 64;
  options.depth = 50;
  options.resolution = glm::ivec2(1000, 1000);

  scene->options = options;

  Camera *camera = new Camera(
      glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f, 3.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f), 40.0,
      (float)scene->options.resolution.x / (float)scene->options.resolution.y,
      0.0, 1000.0);

  scene->setCamera(camera);

  // int mesh_id = scene->AddMesh("assets/monke.obj");
  // MeshInstance instance("monke", mesh_id, glm::mat4(1.0), 0);
  // scene->AddMeshInstance(instance);

  int mesh_id2 = scene->AddMesh("assets/cornell.obj");
  MeshInstance instance2("cornell", mesh_id2, glm::mat4(1.0), 0);
  scene->AddMeshInstance(instance2);

  App app(scene);

  app.run();

  app.finish();
}
